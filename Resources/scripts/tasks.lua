Log:log("Script task.lua initalization started")

function CheckStatus(cellMembers, cellMorale, cellHappyness, taskMoral, taskSeverity)
	return false
end

function SayCompleted(taskName)
	MessageManager:sendMessage("Task " .. taskName .. " completed")
end

function SayFailed(taskName)
	MessageManager:sendMessage("Task " .. taskName .. " failed")
end

function TestMissionSuccess(taskId)
	Log:log("Task "..taskId.." with cash "..cell.cash.." successfully finished!")
	SayCompleted(taskId)
end

function TestMissionFail(taskId)
	Log:log("Task "..taskId.." with cash "..cell.cash.." failed.")
	SayFailed(taskId)
end

function TestMissionAbort(taskId)
	Log:log("Task "..taskId.." with cash "..cell.cash.." aborted")
end

Log:log("Script task.lua initalization finished")