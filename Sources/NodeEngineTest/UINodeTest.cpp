#include "SimpleTest.hpp"
#include "NE_NodeManager.hpp"
#include "NE_Node.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_SingleValues.hpp"
#include "TestUtils.hpp"
#include "TestNodes.hpp"

#include "NUIE_NodeUIManager.hpp"
#include "NUIE_NodeParameters.hpp"
#include "NUIE_NodeCommonParameters.hpp"

using namespace NE;
using namespace NUIE;

namespace UINodeTest
{

class TestNode : public SerializableTestUINode
{
	DYNAMIC_SERIALIZABLE (TestNode);

public:
	TestNode () :
		TestNode (LocString (), Point ())
	{

	}

	TestNode (const LocString& name, const Point& position) :
		SerializableTestUINode (name, position)
	{
		
	}

	virtual void Initialize () override
	{
		RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (SlotId ("in1"), LocString (L"First Input"), NE::ValuePtr (new NE::IntValue (1)), NE::OutputSlotConnectionMode::Single)));
		RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (SlotId ("in2"), LocString (L"Second Input"), NE::ValuePtr (new NE::IntValue (2)), NE::OutputSlotConnectionMode::Single)));
		RegisterUIOutputSlot (UIOutputSlotPtr (new UIOutputSlot (SlotId ("out"), LocString (L"Single Output"))));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv& env) const override
	{
		ValueConstPtr a = EvaluateInputSlot (SlotId ("in1"), env);
		ValueConstPtr b = EvaluateInputSlot (SlotId ("in2"), env);
		int result = IntValue::Get (a) + IntValue::Get (b);
		return ValuePtr (new IntValue (result));
	}

	virtual void RegisterParameters (NodeParameterList& parameterList) const override
	{
		class In1DefaultValueParameter : public SlotDefaultValueNodeParameter<TestNode, NE::IntValue>
		{
		public:
			In1DefaultValueParameter () :
				SlotDefaultValueNodeParameter<TestNode, NE::IntValue> (SlotId ("in1"), NE::LocString (L"In1"), ParameterType::Integer)
			{
			
			}
		};

		UINode::RegisterParameters (parameterList);
		parameterList.AddParameter (NodeParameterPtr (new In1DefaultValueParameter ()));
	}

	virtual void UpdateNodeDrawingImage (NodeUIDrawingEnvironment&, NodeDrawingImage&) const override
	{
	
	}
};

DYNAMIC_SERIALIZATION_INFO (TestNode, 1, "{54A1DE47-8D06-4292-886A-7B8D676271D2}");

class TestNode2 : public SerializableTestUINode
{
public:
	TestNode2 (const LocString& name, const Point& position) :
		SerializableTestUINode (name, position)
	{

	}

	virtual void Initialize () override
	{
		RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (SlotId ("in1"), LocString (L"First Input"), NE::ValuePtr (new NE::IntValue (1)), NE::OutputSlotConnectionMode::Single)));
		RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (SlotId ("in2"), LocString (L"Second Input"), NE::ValuePtr (new NE::IntValue (2)), NE::OutputSlotConnectionMode::Single)));
		RegisterUIOutputSlot (UIOutputSlotPtr (new UIOutputSlot (SlotId ("out"), LocString (L"Single Output"))));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv& env) const override
	{
		ValueConstPtr a = EvaluateInputSlot (SlotId ("in1"), env);
		ValueConstPtr b = EvaluateInputSlot (SlotId ("in2"), env);
		int result = IntValue::Get (a) + IntValue::Get (b);
		return ValuePtr (new IntValue (result));
	}

	virtual void RegisterParameters (NodeParameterList& parameterList) const override
	{
		class In2DefaultValueParameter : public SlotDefaultValueNodeParameter<TestNode2, NE::IntValue>
		{
		public:
			In2DefaultValueParameter () :
				SlotDefaultValueNodeParameter<TestNode2, NE::IntValue> (SlotId ("in2"), NE::LocString (L"In2"), ParameterType::Integer)
			{

			}
		};

		UINode::RegisterParameters (parameterList);
		parameterList.AddParameter (NodeParameterPtr (new In2DefaultValueParameter ()));
	}

	virtual void UpdateNodeDrawingImage (NodeUIDrawingEnvironment&, NodeDrawingImage&) const override
	{

	}
};

class EnumerationParamTestNode : public SerializableTestUINode
{
public:
	enum class MyEnumValue
	{
		AEnum,
		BEnum,
		CEnum
	};

	EnumerationParamTestNode (const LocString& name, const Point& position) :
		SerializableTestUINode (name, position),
		myEnumValue (MyEnumValue::AEnum)
	{

	}

	virtual void Initialize () override
	{
		RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (SlotId ("in1"), LocString (L"First Input"), NE::ValuePtr (new NE::IntValue (1)), NE::OutputSlotConnectionMode::Single)));
		RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (SlotId ("in2"), LocString (L"Second Input"), NE::ValuePtr (new NE::IntValue (2)), NE::OutputSlotConnectionMode::Single)));
		RegisterUIOutputSlot (UIOutputSlotPtr (new UIOutputSlot (SlotId ("out"), LocString (L"Single Output"))));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv& env) const override
	{
		ValueConstPtr a = EvaluateInputSlot (SlotId ("in1"), env);
		ValueConstPtr b = EvaluateInputSlot (SlotId ("in2"), env);
		int result = IntValue::Get (a) + IntValue::Get (b);
		return ValuePtr (new IntValue (result));
	}

	virtual void RegisterParameters (NodeParameterList& parameterList) const override
	{
		class MyEnumerationParameter : public EnumerationNodeParameter<EnumerationParamTestNode>
		{
		public:
			MyEnumerationParameter () :
				EnumerationNodeParameter<EnumerationParamTestNode> (NE::LocString (L"EnumParam"), { NE::LocString (L"AString"), NE::LocString (L"BString"), NE::LocString (L"CString") })
			{

			}

			virtual NE::ValueConstPtr GetValueInternal (const UINodeConstPtr& uiNode) const override
			{
				return NE::ValuePtr (new NE::IntValue ((int) GetTypedNode (uiNode)->GetMyEnumValue ()));
			}

			virtual bool SetValueInternal (UINodeInvalidator& invalidator, NE::EvaluationEnv&, UINodePtr& uiNode, const NE::ValueConstPtr& value) override
			{
				GetTypedNode (uiNode)->SetMyEnumValue ((MyEnumValue) NE::IntValue::Get (value));
				invalidator.InvalidateValueAndDrawing ();
				return true;
			}
		};

		UINode::RegisterParameters (parameterList);
		parameterList.AddParameter (NodeParameterPtr (new MyEnumerationParameter ()));
	}

	virtual void UpdateNodeDrawingImage (NodeUIDrawingEnvironment&, NodeDrawingImage&) const override
	{

	}

	void SetMyEnumValue (MyEnumValue newValue)
	{
		myEnumValue = newValue;
	}

	MyEnumValue GetMyEnumValue () const
	{
		return myEnumValue;
	}

private:
	MyEnumValue myEnumValue;
};

TEST (NodeParametersTest)
{
	TestUIEnvironment env;
	NodeUIManager uiManager (env);
	UINodePtr node (new TestNode (LocString (L"TestNode"), Point (0, 0)));

	NodeUIManagerNodeInvalidator invalidator (uiManager, node);

	ASSERT (uiManager.AddNode (node, NE::EmptyEvaluationEnv) != nullptr);

	NodeParameterList paramList;
	node->RegisterParameters (paramList);
	ValueConstPtr nodeValue = node->Evaluate (NE::EmptyEvaluationEnv);
	ASSERT (IntValue::Get (nodeValue) == 3);

	ASSERT (paramList.GetParameterCount () == 2);
	
	ASSERT (node->GetName ().GetLocalized () == L"TestNode");
	NodeParameterPtr nameParam = paramList.GetParameter (0);
	ASSERT (nameParam->GetName ().GetLocalized () == L"Node Name");
	ValuePtr newNameValue (new StringValue (L"NewNodeName"));
	nameParam->SetValue (invalidator, NE::EmptyEvaluationEnv, node, newNameValue);
	ASSERT (node->GetName ().GetLocalized () == L"NewNodeName");

	NodeParameterPtr in1DefParam = paramList.GetParameter (1);
	ASSERT (in1DefParam->GetName ().GetLocalized () == L"In1");
	ASSERT (IntValue::Get (in1DefParam->GetValue (node)) == 1);
	ValuePtr newIn1Value (new IntValue (5));
	in1DefParam->SetValue (invalidator, NE::EmptyEvaluationEnv, node, newIn1Value);
	ASSERT (IntValue::Get (in1DefParam->GetValue (node)) == 5);
	nodeValue = node->Evaluate (NE::EmptyEvaluationEnv);
	ASSERT (IntValue::Get (nodeValue) == 7);
}

TEST (NodeParametersTest2)
{
	TestUIEnvironment env;
	NodeUIManager uiManager (env);

	UINodePtr node (new TestNode (LocString (L"TestNode"), Point (0, 0)));
	ASSERT (uiManager.AddNode (node, NE::EmptyEvaluationEnv) != nullptr);
	UINodePtr node2 (new TestNode2 (LocString (L"TestNode2"), Point (0, 0)));
	ASSERT (uiManager.AddNode (node2, NE::EmptyEvaluationEnv) != nullptr);

	{
		NodeParameterList paramList;
		RegisterCommonParameters (uiManager, NodeCollection (), paramList);
		ASSERT (paramList.GetParameterCount () == 0);
	}

	{
		NodeParameterList paramList;
		RegisterCommonParameters (uiManager, NodeCollection ({ node->GetId () }), paramList);
		ASSERT (paramList.GetParameterCount () == 2);
		ASSERT (paramList.GetParameter (0)->GetName ().GetLocalized () == L"Node Name");
		ASSERT (paramList.GetParameter (1)->GetName ().GetLocalized () == L"In1");
	}

	{
		NodeParameterList paramList;
		RegisterCommonParameters (uiManager, NodeCollection ({ node2->GetId () }), paramList);
		ASSERT (paramList.GetParameterCount () == 2);
		ASSERT (paramList.GetParameter (0)->GetName ().GetLocalized () == L"Node Name");
		ASSERT (paramList.GetParameter (1)->GetName ().GetLocalized () == L"In2");
	}

	{
		NodeParameterList paramList;
		NodeCollection nodeCollection;
		nodeCollection.Insert (node->GetId ());
		nodeCollection.Insert (node2->GetId ());
		RegisterCommonParameters (uiManager, nodeCollection, paramList);
		ASSERT (paramList.GetParameterCount () == 1);
		ASSERT (paramList.GetParameter (0)->GetName ().GetLocalized () == L"Node Name");
	}
}

TEST (NodeParametersTest3)
{
	TestUIEnvironment env;
	NodeUIManager uiManager (env);

	UINodePtr node (new TestNode (LocString (L"TestNode"), Point (0, 0)));
	ASSERT (uiManager.AddNode (node, NE::EmptyEvaluationEnv) != nullptr);
	UINodePtr node2 (new TestNode (LocString (L"TestNode"), Point (0, 0)));
	ASSERT (uiManager.AddNode (node2, NE::EmptyEvaluationEnv) != nullptr);

	{
		NodeParameterList paramList;
		NodeCollection nodeCollection;
		nodeCollection.Insert (node->GetId ());
		nodeCollection.Insert (node2->GetId ());
		RegisterCommonParameters (uiManager, nodeCollection, paramList);
		ASSERT (paramList.GetParameterCount () == 2);
		ASSERT (paramList.GetParameter (0)->GetName ().GetLocalized () == L"Node Name");
		ASSERT (paramList.GetParameter (1)->GetName ().GetLocalized () == L"In1");
		ValuePtr newName (new StringValue (L"NewName"));
		ApplyCommonParameter (uiManager, EmptyEvaluationEnv, nodeCollection, paramList.GetParameter (0), newName);
		ASSERT (node->GetName ().GetLocalized () == L"NewName");
		ASSERT (node2->GetName ().GetLocalized () == L"NewName");
	}
}

TEST (NodeParametersTest4)
{
	TestUIEnvironment env;
	NodeUIManager uiManager (env);

	std::shared_ptr<EnumerationParamTestNode> node (new EnumerationParamTestNode (LocString (L"TestNode"), Point (0, 0)));
	ASSERT (uiManager.AddNode (node, NE::EmptyEvaluationEnv) != nullptr);

	{
		NodeParameterList paramList;
		NodeCollection nodeCollection ({ node->GetId () });
		RegisterCommonParameters (uiManager, nodeCollection, paramList);
		ASSERT (paramList.GetParameterCount () == 2);
		ASSERT (paramList.GetParameter (0)->GetName ().GetLocalized () == L"Node Name");
		ASSERT (paramList.GetParameter (1)->GetName ().GetLocalized () == L"EnumParam");
		std::vector<NE::LocString> choices = paramList.GetParameter (1)->GetValueChoices ();
		ASSERT (choices.size () == 3);
		ASSERT (choices[0].GetLocalized () == L"AString");
		ASSERT (choices[1].GetLocalized () == L"BString");
		ASSERT (choices[2].GetLocalized () == L"CString");

		ASSERT (node->GetMyEnumValue () == EnumerationParamTestNode::MyEnumValue::AEnum);

		ValuePtr newValue (new IntValue (1));
		ApplyCommonParameter (uiManager, EmptyEvaluationEnv, nodeCollection, paramList.GetParameter (1), newValue);
		ASSERT (node->GetMyEnumValue () == EnumerationParamTestNode::MyEnumValue::BEnum);
	}
}

TEST (DuplicateNodeTest)
{
	TestUIEnvironment env;
	NodeUIManager uiManager (env);

	UINodePtr node1 = uiManager.AddNode (UINodePtr (new TestNode (LocString (L"TestNode"), Point (0, 0))), NE::EmptyEvaluationEnv);
	UINodePtr node2 = uiManager.AddNode (UINodePtr (new TestNode (LocString (L"TestNode2"), Point (0, 0))), NE::EmptyEvaluationEnv);

	NE::NodeCollection nodesToDuplicate ({ node1->GetId (), node2->GetId () });
	NE::NodeCollection duplicatedNodes = uiManager.Duplicate (nodesToDuplicate);

	ASSERT (duplicatedNodes.Count () == 2);
	UINodePtr node3 = uiManager.GetNode (duplicatedNodes.Get (0));
	UINodePtr node4 = uiManager.GetNode (duplicatedNodes.Get (1));

	ASSERT (node3 != nullptr);
	ASSERT (node3->GetId () != NullNodeId);
	ASSERT (node3->GetName ().GetLocalized () == L"TestNode");
	ASSERT (uiManager.ContainsNode (node3->GetId ()));

	ASSERT (node4 != nullptr);
	ASSERT (node4->GetId () != NullNodeId);
	ASSERT (node4->GetName ().GetLocalized () == L"TestNode2");
	ASSERT (uiManager.ContainsNode (node4->GetId ()));
}

}
