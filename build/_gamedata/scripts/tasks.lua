Log.Log("Script task.lua initalization started")

function CheckStatus(cellMembers, cellMorale, cellHappyness, taskMoral, taskSeverity)
	return true
end

function TestMissionSuccess()
	Log.Log("Task in city "..cell.town.name.." successfully finished!")
end

function TestMissionFail()
	Log.Log("Task in city "..cell.town.name.." failed.")
end

function TestMissionAbort()
	Log.Log("Task in city "..cell.town.name.." aborted")
end

Log.Log("Script task.lua initalization finished")