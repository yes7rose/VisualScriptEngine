#include "NUIE_UndoHandler.hpp"
#include "NE_Debug.hpp"

namespace NUIE
{

static void AddStateToStack (const NE::NodeManager& nodeManager, std::vector<std::shared_ptr<NE::NodeManager>>& stack)
{
	std::shared_ptr<NE::NodeManager> undoState (new NE::NodeManager ());
	NE::NodeManager::Clone (nodeManager, *undoState.get ());
	stack.push_back (undoState);
}

UndoHandler::UndoHandler ()
{

}

void UndoHandler::AddUndoStep (const NE::NodeManager& nodeManager)
{
	redoStack.clear ();
	AddStateToStack (nodeManager, undoStack);
}

bool UndoHandler::CanUndo () const
{
	return !undoStack.empty ();
}

bool UndoHandler::CanRedo () const
{
	return !redoStack.empty ();
}

bool UndoHandler::Undo (NE::NodeManager& targetNodeManager, NE::UpdateEventHandler& eventHandler)
{
	if (!CanUndo ()) {
		return false;
	}

	AddStateToStack (targetNodeManager, redoStack);

	std::shared_ptr<NE::NodeManager> undoState = undoStack.back ();
	undoStack.pop_back ();

	return NE::NodeManagerMerge::UpdateNodeManager (*undoState.get (), targetNodeManager, eventHandler);
}

bool UndoHandler::Redo (NE::NodeManager& targetNodeManager, NE::UpdateEventHandler& eventHandler)
{
	if (!CanRedo ()) {
		return false;
	}

	AddStateToStack (targetNodeManager, undoStack);

	std::shared_ptr<NE::NodeManager> redoState = redoStack.back ();
	redoStack.pop_back ();

	return NE::NodeManagerMerge::UpdateNodeManager (*redoState.get (), targetNodeManager, eventHandler);
}

void UndoHandler::Clear ()
{
	undoStack.clear ();
	redoStack.clear ();
}

}
