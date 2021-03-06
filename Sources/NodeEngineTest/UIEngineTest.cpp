#include "SimpleTest.hpp"
#include "NE_NodeManager.hpp"
#include "NE_Node.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_SingleValues.hpp"
#include "TestUtils.hpp"
#include "TestNodes.hpp"

#include "NUIE_NodeUIManager.hpp"
#include "NUIE_DrawingContext.hpp"

using namespace NE;
using namespace NUIE;

namespace NodeUIEngineTest
{

class TestInputSlot : public UIInputSlot
{
public:
	TestInputSlot (const SlotId& id, const LocString& name) :
		UIInputSlot (id, name, ValuePtr (new IntValue (0)), NE::OutputSlotConnectionMode::Single)
	{

	}
};

class TestOutputSlot : public UIOutputSlot
{
public:
	TestOutputSlot (const SlotId& id, const LocString& name) :
		UIOutputSlot (id, name)
	{

	}
};

class TestNode : public SerializableTestUINode
{
public:
	TestNode (const Point& position) :
		SerializableTestUINode (LocString (L"Test Node"), position)
	{
		
	}

	virtual void Initialize () override
	{
		RegisterUIInputSlot (UIInputSlotPtr (new TestInputSlot (SlotId ("in1"), LocString (L"First Input"))));
		RegisterUIInputSlot (UIInputSlotPtr (new TestInputSlot (SlotId ("in2"), LocString (L"Second Input"))));
		RegisterUIOutputSlot (UIOutputSlotPtr (new TestOutputSlot (SlotId ("out"), LocString (L"Single Output"))));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv&) const override
	{
		return ValuePtr (new IntValue (42));
	}

	virtual void UpdateNodeDrawingImage (NodeUIDrawingEnvironment&, NodeDrawingImage&) const override
	{
	
	}
};

TEST (UIManagerBaseTest)
{
	TestUIEnvironment env;
	NodeUIManager uiManager (env);

	UINodePtr node1 (new TestNode (Point (0.0, 0.0)));
	UINodePtr node2 (new TestNode (Point (0.0, 0.0)));

	ASSERT (uiManager.AddNode (node1, NE::EmptyEvaluationEnv) != nullptr);
	ASSERT (uiManager.AddNode (node2, NE::EmptyEvaluationEnv) != nullptr);

	ASSERT (node1->GetName ().GetLocalized () == L"Test Node");
	ASSERT (node1->GetUIInputSlot (SlotId ("in1"))->GetName ().GetLocalized () == L"First Input");
	ASSERT (node1->GetUIInputSlot (SlotId ("in2"))->GetName ().GetLocalized () == L"Second Input");
	ASSERT (node1->GetUIOutputSlot (SlotId ("out"))->GetName ().GetLocalized () == L"Single Output");

	ASSERT (uiManager.ConnectOutputSlotToInputSlot (node1->GetUIOutputSlot (SlotId ("out")), node2->GetUIInputSlot (SlotId ("in1"))));
	ASSERT (uiManager.ConnectOutputSlotToInputSlot (node1->GetUIOutputSlot (SlotId ("out")), node2->GetUIInputSlot (SlotId ("in2"))));
	ASSERT (uiManager.DeleteNode (node1->GetId (), NE::EmptyEvaluationEnv, env));
}

TEST (ViewBoxTest)
{
	// model : 6 x 4

	{
		ViewBox viewBox (Point (1.0, 2.0), 1.0);
		ASSERT (IsEqual (viewBox.ModelToView (Point (0.0, 0.0)), Point (1.0, 2.0)));
		ASSERT (IsEqual (viewBox.ModelToView (Point (1.0, 1.0)), Point (2.0, 3.0)));
		ASSERT (IsEqual (viewBox.ViewToModel (Point (1.0, 2.0)), Point (0.0, 0.0)));
		ASSERT (IsEqual (viewBox.ViewToModel (Point (2.0, 3.0)), Point (1.0, 1.0)));
	}
	{
		ViewBox viewBox (Point (1.0, 2.0), 0.5);
		ASSERT (IsEqual (viewBox.ModelToView (Point (0.0, 0.0)), Point (1.0, 2.0)));
		ASSERT (IsEqual (viewBox.ModelToView (Point (2.0, 2.0)), Point (2.0, 3.0)));
		ASSERT (IsEqual (viewBox.ModelToView (Point (6.0, 4.0)), Point (4.0, 4.0)));
		ASSERT (IsEqual (viewBox.ViewToModel (Point (1.0, 2.0)), Point (0.0, 0.0)));
		ASSERT (IsEqual (viewBox.ViewToModel (Point (2.0, 3.0)), Point (2.0, 2.0)));
		ASSERT (IsEqual (viewBox.ViewToModel (Point (4.0, 4.0)), Point (6.0, 4.0)));
	}
}

TEST (ViewBoxScaleFixPointTest)
{
	std::vector<Point> points = {
		Point (0.0, 0.0),
		Point (2.0, 0.0),
		Point (0.0, 3.0),
		Point (2.0, 3.0)
	};

	for (double scale = 0.1; scale < 1.1; scale += 0.1) {
		for (const Point& mousePoint : points) {
			ViewBox viewBox (Point (1.0, 2.0), 1.0);
			Point origModelPoint = viewBox.ViewToModel (mousePoint);
			viewBox.SetScale (scale, mousePoint);
			ASSERT (IsEqual (viewBox.ViewToModel (mousePoint), origModelPoint));
		}
	}
}

TEST (ViewBoxFitTest)
{
	{
		ViewBox viewBox = FitRectToSize (Size (10.0, 10.0), 0.0, Rect (0.0, 0.0, 10.0, 10.0));
		ASSERT (IsEqual (viewBox.GetOffset (), Point (0.0, 0.0)));
		ASSERT (IsEqual (viewBox.GetScale (), 1.0));
	}

	{
		ViewBox viewBox = FitRectToSize (Size (10.0, 10.0), 0.0, Rect (0.0, 0.0, 20.0, 20.0));
		ASSERT (IsEqual (viewBox.GetOffset (), Point (0.0, 0.0)));
		ASSERT (IsEqual (viewBox.GetScale (), 0.5));
	}

	{
		ViewBox viewBox = FitRectToSize (Size (10.0, 10.0), 0.0, Rect (0.0, 0.0, 5.0, 5.0));
		ASSERT (IsEqual (viewBox.GetOffset (), Point (0.0, 0.0)));
		ASSERT (IsEqual (viewBox.GetScale (), 2.0));
	}

	{
		ViewBox viewBox = FitRectToSize (Size (3.0, 2.0), 0.0, Rect (0.0, 0.0, 3.0, 2.0));
		ASSERT (IsEqual (viewBox.GetOffset (), Point (0.0, 0.0)));
		ASSERT (IsEqual (viewBox.GetScale (), 1.0));
	}

	{
		ViewBox viewBox = FitRectToSize (Size (3.0, 2.0), 0.0, Rect (0.0, 0.0, 2.0, 3.0));
		double expectedScale = 2.0 / 3.0;
		ASSERT (IsEqual (viewBox.GetOffset (), Point ((3.0 - (2.0 * expectedScale)) / 2.0, 0.0)));
		ASSERT (IsEqual (viewBox.GetScale (), expectedScale));
	}

	{
		ViewBox viewBox = FitRectToSize (Size (2.0, 3.0), 0.0, Rect (0.0, 0.0, 2.0, 3.0));
		ASSERT (IsEqual (viewBox.GetOffset (), Point (0.0, 0.0)));
		ASSERT (IsEqual (viewBox.GetScale (), 1.0));
	}

	{
		ViewBox viewBox = FitRectToSize (Size (2.0, 3.0), 0.0, Rect (0.0, 0.0, 3.0, 2.0));
		double expectedScale = 2.0 / 3.0;
		ASSERT (IsEqual (viewBox.GetOffset (), Point (0.0, (3.0 - (2.0 * expectedScale)) / 2.0)));
		ASSERT (IsEqual (viewBox.GetScale (), expectedScale));
	}

	{
		ViewBox viewBox = FitRectToSize (Size (3.0, 2.0), 0.2, Rect (0.0, 0.0, 3.0, 2.0));
		double expectedScale = (2.0 - 0.4) / 2.0;
		ASSERT (IsEqual (viewBox.GetOffset (), Point ((3.0 - (3.0 * expectedScale)) / 2.0, 0.2)));
		ASSERT (IsEqual (viewBox.GetScale (), expectedScale));
	}

	{
		ViewBox viewBox = FitRectToSize (Size (3.0, 2.0), 0.2, Rect (0.0, 0.0, 2.0, 3.0));
		double expectedScale = (2.0 - 0.4) / 3.0;
		ASSERT (IsEqual (viewBox.GetOffset (), Point ((3.0 - (2.0 * expectedScale)) / 2.0, 0.2)));
		ASSERT (IsEqual (viewBox.GetScale (), expectedScale));
	}

	{
		ViewBox viewBox = FitRectToSize (Size (2.0, 3.0), 0.2, Rect (0.0, 0.0, 2.0, 3.0));
		double expectedScale = (2.0 - 0.4) / 2.0;
		ASSERT (IsEqual (viewBox.GetOffset (), Point (0.2, (3.0 - (3.0 * expectedScale)) / 2.0)));
		ASSERT (IsEqual (viewBox.GetScale (), expectedScale));
	}

	{
		ViewBox viewBox = FitRectToSize (Size (2.0, 3.0), 0.2, Rect (0.0, 0.0, 3.0, 2.0));
		double expectedScale = (2.0 - 0.4) / 3.0;
		ASSERT (IsEqual (viewBox.GetOffset (), Point (0.2, (3.0 - (2.0 * expectedScale)) / 2.0)));
		ASSERT (IsEqual (viewBox.GetScale (), expectedScale));
	}
}

}
