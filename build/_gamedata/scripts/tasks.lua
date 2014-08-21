Log.Log("Script task.lua initalization started")

function CheckStatus(cellMembers, cellMorale, cellHappyness, taskMoral, taskSeverity)
	return true
end

function TestMissionSuccess(taskId)
	Log.Log("Task "..taskId.." in city "..cell.town.name.." successfully finished!")
end

function TestMissionFail(taskId)
	Log.Log("Task "..taskId.." in city "..cell.town.name.." failed.")
end

function TestMissionAbort(taskId)
	Log.Log("Task "..taskId.." in city "..cell.town.name.." aborted")
end

Log.Log("Script task.lua initalization finished")