-- Определяем, считать задание выполненным или провеленным
function CheckTaskStatus(cellInfo, taskInfo)
	local membersIsEnough = cellInfo.membersCount >= taskInfo.severity * 10.0
	local moraleIsNorm = math.abs(cellInfo.morale - taskInfo.moraleLevel) < 0.5
	return membersIsEnough and moraleIsNorm
end

-- определяем будет ли показываться задание для данной ячейки
function IsShowTaskInList(cell, taskInfo)
	local cellInfo = cell:getInfo()

	-- задачи для туториала
	if World:getTutorialState() == "StartFirstTask" then
		if taskInfo.id == "tutorial_Recrutment" then
			return true
		else
			return false
		end
	elseif taskInfo.id == "tutorial_Recrutment" then
		return false
	end

	if World:getTutorialState() == "ReadyToFirstRealWork" then
		if taskInfo.id == "tutorial_RealWork" then
			return true
		else
			return false
		end
	elseif taskInfo.id == "tutorial_RealWork" then
		return false
	end

	-- для первой альфы делаем не очень хорошие проверки
	if taskInfo.id == "alpha1_Recrutment1" then
		return cellInfo.cash <= 50000
	elseif taskInfo.id == "alpha1_Recrutment2" then
		return cellInfo.cash > 50000 
	elseif taskInfo.id == "alpha1_BankRobbery" then
		return cellInfo.membersCount >= 10
	end

	return true
end

-- отправляем игроку сообщение, о том что задание выполнено
function SayCompleted(taskName)
	MessageManager:sendMessage("Task " .. taskName .. " completed")
end

-- отправляем игроку сообщение, о том что задание провалено
function SayFailed(taskName)
	MessageManager:sendMessage("Task " .. taskName .. " failed")
end

function MissionSuccess_Test(cellInfo, taskInfo)
	Log:log("Task "..taskInfo.id.." successfully finished!")

	-- увеличивем преданность на 10% от текущего
	cellInfo.contantement = cellInfo.contantement + (cellInfo.contantement * 0.1)
	cellInfo.cash = cellInfo.cash + 200
	cellInfo.membersCount = cellInfo.membersCount + 1

	-- проверяем на предельное значение
	if cellInfo.contantement > 1.0 then
		cellInfo.contantement = 1
	end

	SayCompleted(taskInfo.title)
end

function MissionFail_Test(cellInfo, taskInfo)
	Log:log("Task "..taskInfo.id.." failed.")

	-- уменьшаем преданность на 10% от текущего
	cellInfo.contantement = cellInfo.contantement - (cellInfo.contantement * 0.1)

	-- убираем одного члена ячейки, если их болльше 2
	if cellInfo.membersCount > 2 then
		cellInfo.membersCount = cellInfo.membersCount - 1
	end
	
	SayFailed(taskInfo.title)
end

function MissionAbort_Test(cellInfo, taskInfo)
	Log:log("Task "..taskInfo.title.." aborted")

	-- уменьшаем довольство на 20% от текущего
	cellInfo.contantement = cellInfo.contantement - (cellInfo.contantement * 0.2)
end

function MissionSuccess_InvestigatorTest(cellInfo, taskInfo)
	MessageManager:sendMessage("Investigation launched")
	World:addInvestigatorByInfo(cellInfo)
end

function MissionFail_InvestigatorTest(cellInfo, taskInfo)
	SayFailed(taskInfo.title)
	MessageManager:sendMessage("Investigation launched")
	World:addInvestigatorByInfo(cellInfo)
end

function MissionSuccess_CheatMission(cellInfo, taskInfo)
	SayCompleted("CheatMission")
	cellInfo.membersCount = 100
	cellInfo.cash = 100000
end

function MissionSuccess_TutorialRecrutment(cellInfo, taskInfo)
	SayCompleted(taskInfo.title)

	cellInfo.membersCount = cellInfo.membersCount + 6

	World:runTutorialState("AfterFirstTaskFinished")
end

function MissionSuccess_TutorialFirstRealWork(cellInfo, taskInfo)
	SayCompleted(taskInfo.title)

	-- мораль ячейки приближается к морали задания
	cellInfo.morale = cellInfo.morale + (taskInfo.moraleLevel - cellInfo.morale) * 0.3

	-- увеличивем преданность на 10% от текущего
	cellInfo.contantement = cellInfo.contantement + (cellInfo.contantement * 0.1)
	if (cellInfo.contantement > 1.0) then cellInfo.contantement = 1.0 end

	cellInfo.cash = cellInfo.cash + math.random(150, 200) * 1000

	World:runTutorialState("AfterRealWorkDone")
end

function MissionSuccess_Alpha1Recrutment(cellInfo, taskInfo)
	SayCompleted(taskInfo.title)

	cellInfo.membersCount = cellInfo.membersCount + math.random(2, 5)
end

function MissionFail_Alpha1Recrutment(cellInfo, taskInfo)
	SayFailed(taskInfo.title)
end

function MissionSuccess_Alpha1BankRobbery(cellInfo, taskInfo)
	SayCompleted(taskInfo.title)

	-- мораль ячейки приближается к морали задания
	cellInfo.morale = cellInfo.morale + (taskInfo.moraleLevel - cellInfo.morale) * 0.3

	-- увеличивем преданность на 10% от текущего
	cellInfo.contantement = cellInfo.contantement + (cellInfo.contantement * 0.1)
	if (cellInfo.contantement > 1.0) then cellInfo.contantement = 1.0 end

	cellInfo.cash = cellInfo.cash + math.random(150, 200) * 1000
end

function MissionFail_Alpha1BankRobbery(cellInfo, taskInfo)
	SayFailed(taskInfo.title)

	-- мораль ячейки приближается к морали задания
	cellInfo.morale = cellInfo.morale + (taskInfo.moraleLevel - cellInfo.morale) * 0.3

	-- уменьшаем преданность на 25-35% от текущего
	cellInfo.contantement = cellInfo.contantement - (cellInfo.contantement * 0.01 * math.random(25, 35))

	cellInfo.membersCount = cellInfo.membersCount - math.random(0, 3)
end
