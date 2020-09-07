#include "SimpleTest.hpp"
#include "NE_MemoryStream.hpp"
#include "NUIE_NodeEditor.hpp"
#include "BI_BuiltInNodes.hpp"
#include "VisualTestFramework.hpp"

using namespace NE;
using namespace NUIE;
using namespace BI;

namespace NodeEditorTest
{

TEST (NodeEditorNeedToSaveTest)
{
	NodeEditorTestEnv env (GetDefaultSkinParams ());
	ASSERT (!env.nodeEditor.NeedToSave ());

	env.nodeEditor.AddNode (UINodePtr (new IntegerUpDownNode (LocString (L"Integer"), Point (0.0, 0.0), 0, 1)));
	ASSERT (env.nodeEditor.NeedToSave ());
	
	MemoryOutputStream outputStream;
	ASSERT (env.nodeEditor.Save (outputStream));
	ASSERT (!env.nodeEditor.NeedToSave ());
	
	env.nodeEditor.New ();
	ASSERT (!env.nodeEditor.NeedToSave ());
	env.nodeEditor.AddNode (UINodePtr (new IntegerUpDownNode (LocString (L"Integer"), Point (0.0, 0.0), 0, 1)));
	ASSERT (env.nodeEditor.NeedToSave ());

	MemoryInputStream inputStream (outputStream.GetBuffer ());
	ASSERT (env.nodeEditor.Open (inputStream));
	ASSERT (!env.nodeEditor.NeedToSave ());	
	env.nodeEditor.AddNode (UINodePtr (new IntegerUpDownNode (LocString (L"Integer"), Point (0.0, 0.0), 0, 1)));
	ASSERT (env.nodeEditor.NeedToSave ());
}

TEST (NodeEditorGetInfoTest)
{
	NodeEditorTestEnv env (GetDefaultSkinParams ());

	ViewBox testViewBox (Point (50, 50), 2.0);
	env.nodeEditor.SetViewBox (testViewBox);

	env.nodeEditor.AddNode (UINodePtr (new IntegerUpDownNode (LocString (L"Integer"), Point (100.0, 100.0), 0, 1)));
	env.nodeEditor.AddNode (UINodePtr (new MultiLineViewerNode (LocString (L"Viewer"), Point (200.0, 200.0), 5)));

	NodeEditorInfo info = env.nodeEditor.GetInfo ();

	ASSERT (info.nodes.size () == 2);
	ASSERT (info.nodes[0].name == L"Integer");
	ASSERT (info.nodes[0].inputSlots.size () == 0);
	ASSERT (info.nodes[0].outputSlots.size () == 1);
}

}
