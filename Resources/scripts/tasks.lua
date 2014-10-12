Log:log("Script task.lua initalization started")

-- Определяем, считать задание выполненным или провеленным
function CheckStatus(cellInfo, taskInfo)
	local membersIsEnough = cellInfo.membersCount > taskInfo.severity * 10.0
	local moraleIsNorm = math.abs(cellInfo.morale - taskInfo.moraleLevel) < 0.5
	return membersIsEnough and moraleIsNorm
end

-- отправляем игроку сообщение, о том что задание выполнено
function SayCompleted(taskName)
	MessageManager:sendMessage("Task " .. taskName .. " completed")
end

-- отправляем игроку сообщение, о том что задание провалено
function SayFailed(taskName)
	MessageManager:sendMessage("Task " .. taskName .. " failed")
end

function TestMissionSuccess(cellInfo, taskInfo)
	Log:log("Task "..taskInfo.id.." successfully finished!"..cellInfo.morale)

	-- увеличивем довольство на 10% от текущего
	cellInfo.contantement = cellInfo.contantement + (cellInfo.contantement * 0.1)
	cellInfo.cash = cellInfo.cash + 200
	cellInfo.membersCount = cellInfo.membersCount + 1

	-- проверяем на предельное значение
	if cellInfo.contantement > 1.0 then
		cellInfo.contantement = 1
	end

	SayCompleted(taskInfo.id)
end

function TestMissionFail(cellInfo, taskInfo)
	Log:log("Task "..taskInfo.id.." failed."..cellInfo.morale)

	-- уменьшаем довольство на 10% от текущего
	cellInfo.contantement = cellInfo.contantement - (cellInfo.contantement * 0.1)

	-- убираем одного члена ячейки, если их болльше 2
	if cellInfo.membersCount > 2 then
		cellInfo.membersCount = cellInfo.membersCount - 1
	end
	
	SayFailed(taskInfo.id)
end

function TestMissionAbort(cellInfo, taskInfo)
	Log:log("Task "..taskInfo.id.." aborted")

	-- уменьшаем довольство на 20% от текущего
	cellInfo.contantement = cellInfo.contantement - (cellInfo.contantement * 0.2)
end

function  MissionFailInvestigatorTest(cellInfo, taskInfo)
	SayFailed(taskInfo.id)
	MessageManager:sendMessage("Investigation launched")
	World:AddInvestigatorByInfo(cellInfo)
end

function CheatMissionSuccess(cellInfo, taskInfo)
	SayCompleted("CheatMission")
	cellInfo.membersCount = 100
	cellInfo.cash = 100000
end

Log:log("Script task.lua initalization finished")