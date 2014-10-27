-- Определяем, считать задание выполненным или провеленным
function CheckTaskStatus(cellInfo, taskInfo)
	local successChance = CalcTaskSuccessChance(cellInfo, taskInfo)
	return math.random(0, 1000) / 1000.0 < successChance
end

-- возвращает вероятность выполнения конкретного задания конкретной ячейкой
function CalcTaskSuccessChance(cellInfo, taskInfo)
	local successChance = 1 - math.abs(cellInfo.morale - taskInfo.moraleLevel)
	return successChance
end

-- определяем будет ли показываться задание для данной ячейки
function IsShowTaskInList(cell, taskInfo)
	local cellInfo = cell:getInfo()

	-- задачи для туториала
	if World:getTutorialState() == "WaitingForStartFirstTask" then
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

function TryToStartInvestigation(cell)
	local cellInfo = cell:getInfo()
	local investigationChance = GetInvestigationChance(cell)
	Log:log("Investigation chance = " .. investigationChance)
	if math.random(0, 1000) / 1000.0 < investigationChance then
		World:addInvestigatorByInfo(cellInfo)
		Log:log("Investigation started")
	end
end

-- отправляем игроку сообщение, о том что задание выполнено
function SayCompleted(taskName)
	MessageManager:sendMessage("Task " .. taskName .. " completed")
end

-- отправляем игроку сообщение, о том что задание провалено
function SayFailed(taskName)
	MessageManager:sendMessage("Task " .. taskName .. " failed")
end

function MissionSuccess_Test(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayCompleted(taskInfo.title)

	-- увеличивем преданность на 10% от текущего
	cellInfo.devotion = cellInfo.devotion + (cellInfo.devotion * 0.1)
	cellInfo.cash = cellInfo.cash + 200
	cellInfo.membersCount = cellInfo.membersCount + 1

	-- проверяем на предельное значение
	if cellInfo.devotion > 1.0 then
		cellInfo.devotion = 1
	end
end

function MissionFail_Test(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayFailed(taskInfo.title)

	-- уменьшаем преданность на 10% от текущего
	cellInfo.devotion = cellInfo.devotion - (cellInfo.devotion * 0.1)

	-- убираем одного члена ячейки, если их болльше 2
	if cellInfo.membersCount > 2 then
		cellInfo.membersCount = cellInfo.membersCount - 1
	end
end

function MissionAbort_Test(cell, taskInfo)
	local cellInfo = cell:getInfo()
	Log:log("Task "..taskInfo.title.." aborted")

	-- уменьшаем довольство на 20% от текущего
	cellInfo.devotion = cellInfo.devotion - (cellInfo.devotion * 0.2)
end

function MissionSuccess_InvestigatorTest(cell, taskInfo)
	local cellInfo = cell:getInfo()
	MessageManager:sendMessage("Investigation launched")
	World:addInvestigatorByInfo(cellInfo)
end

function MissionFail_InvestigatorTest(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayFailed(taskInfo.title)
	MessageManager:sendMessage("Investigation launched")
	World:addInvestigatorByInfo(cellInfo)
end

function MissionSuccess_CheatMission(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayCompleted("CheatMission")
	cellInfo.membersCount = 100
	cellInfo.cash = 100000
end

function MissionSuccess_TutorialRecrutment(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayCompleted(taskInfo.title)

	cellInfo.membersCount = cellInfo.membersCount + 6

	World:runTutorialFuncton("AfterFirstTaskFinished")
end

function MissionSuccess_TutorialFirstRealWork(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayCompleted(taskInfo.title)

	-- мораль ячейки приближается к морали задания
	cellInfo.morale = cellInfo.morale + (taskInfo.moraleLevel - cellInfo.morale) * 0.3

	-- увеличивем преданность на 10% от текущего
	cellInfo.devotion = cellInfo.devotion + (cellInfo.devotion * 0.1)
	if (cellInfo.devotion > 1.0) then cellInfo.devotion = 1.0 end

	cellInfo.cash = cellInfo.cash + math.random(150, 200) * 1000

	World:runTutorialFuncton("AfterRealWorkDone")
end

function MissionSuccess_Alpha1Recrutment(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayCompleted(taskInfo.title)

	cellInfo.membersCount = cellInfo.membersCount + math.random(2, 5)
end

function MissionFail_Alpha1Recrutment(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayFailed(taskInfo.title)
end

function MissionSuccess_Alpha1BankRobbery(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayCompleted(taskInfo.title)

	-- мораль ячейки приближается к морали задания
	cellInfo.morale = cellInfo.morale + (taskInfo.moraleLevel - cellInfo.morale) * 0.3

	-- увеличивем преданность на 10% от текущего
	cellInfo.devotion = cellInfo.devotion + (cellInfo.devotion * 0.1)
	if (cellInfo.devotion > 1.0) then cellInfo.devotion = 1.0 end

	cellInfo.cash = cellInfo.cash + math.random(150, 200) * 1000
end

function MissionFail_Alpha1BankRobbery(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayFailed(taskInfo.title)

	-- мораль ячейки приближается к морали задания
	cellInfo.morale = cellInfo.morale + (taskInfo.moraleLevel - cellInfo.morale) * 0.3

	-- уменьшаем преданность на 25-35% от текущего
	cellInfo.devotion = cellInfo.devotion - (cellInfo.devotion * 0.01 * math.random(25, 35))

	cellInfo.membersCount = cellInfo.membersCount - math.random(0, 3)

	TryToStartInvestigation(cell)
end
