-- Определяем, считать задание выполненным или провеленным
function CheckTaskStatus(cellInfo, taskInfo)
	local successChance = CalcTaskSuccessChance(cellInfo, taskInfo)
	return math.random(0, 1000) / 1000.0 < successChance
end

-- возвращает вероятность выполнения конкретного задания конкретной ячейкой
function CalcTaskSuccessChance(cellInfo, taskInfo)
	local successChance = 1 - math.abs(cellInfo.morale - taskInfo.morale)
	successChance = successChance * math.max(cellInfo.devotion, 0.2)
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

	if (cellInfo.membersCount <= taskInfo.needMembers) then
		return false
	end

	if (cellInfo.cash <= taskInfo.needCash) then
		return false
	end

	return true
end

function TryToStartInvestigation(cell)
	local cellInfo = cell:getInfo()
	local investigationChance = GetInvestigationChance(cell)
	Log:log("Investigation chance = " .. investigationChance)
	if math.random(0, 1000) / 1000.0 < investigationChance then
		World:addInvestigatorByCellUid(cell:getUid())
		Log:log("Investigation started")
	end
end

function CalcRecruitmentParams(cellInfo, addedMembersCount, newMembersMorale)
	local newMorale = (cellInfo.membersCount * cellInfo.morale + addedMembersCount * newMembersMorale) / (cellInfo.membersCount + addedMembersCount)
	local newDevotion = cellInfo.devotion * math.abs(cellInfo.morale - newMorale)
	return newMorale, newDevotion
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
	World:addInvestigatorByCellUid(cell:getUid())
end

function MissionFail_InvestigatorTest(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayFailed(taskInfo.title)
	MessageManager:sendMessage("Investigation launched")
	World:addInvestigatorByCellUid(cell:getUid())
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
	cellInfo.morale = cellInfo.morale + (taskInfo.morale - cellInfo.morale) * 0.3

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
	cellInfo.morale = cellInfo.morale + (taskInfo.morale - cellInfo.morale) * 0.3

	-- увеличивем преданность на 10% от текущего
	cellInfo.devotion = cellInfo.devotion + (cellInfo.devotion * 0.1)
	if (cellInfo.devotion > 1.0) then cellInfo.devotion = 1.0 end

	cellInfo.cash = cellInfo.cash + math.random(150, 200) * 1000
end

function MissionFail_Alpha1BankRobbery(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayFailed(taskInfo.title)

	-- мораль ячейки приближается к морали задания
	cellInfo.morale = cellInfo.morale + (taskInfo.morale - cellInfo.morale) * 0.3

	-- уменьшаем преданность на 25-35% от текущего
	cellInfo.devotion = cellInfo.devotion - (cellInfo.devotion * 0.01 * math.random(25, 35))

	cellInfo.membersCount = cellInfo.membersCount - math.random(0, 3)

	TryToStartInvestigation(cell)
end

function AcceptRecrutmentParams(cell, taskInfo, newMembersCount, fameAdd)
	local cellInfo = cell:getInfo()
	local newMembersMorale = taskInfo.morale
	local newMorale, newDevotion = CalcRecruitmentParams(cellInfo, newMembersCount, newMembersMorale)
	cellInfo.membersCount = cellInfo.membersCount + newMembersCount
	cellInfo.morale = newMorale
	cellInfo.devotion = newDevotion
	cellInfo.fame = cellInfo.fame + fameAdd
	if cellInfo.fame > 1.0 then cellInfo.fame = 1.0 end
end

function MissionSuccess_Alpha1Recrutment_university(cell, taskInfo)
	local newMembersCount = math.random(2, 4)
	SayCompleted(taskInfo.title)
	AcceptRecrutmentParams(cell, taskInfo, newMembersCount, 0.05)
end

function MissionFail_Alpha1Recrutment_university(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayFailed(taskInfo.title)
	cellInfo.fame = cellInfo.fame + 0.1
	if cellInfo.fame > 1.0 then cellInfo.fame = 1.0 end
end

function MissionAbort_Alpha1Recrutment_university(cell, taskInfo)
	local cellInfo = cell:getInfo()
	cellInfo.fame = cellInfo.fame + 0.1
	if cellInfo.fame > 1.0 then cellInfo.fame = 1.0 end
end

function MissionSuccess_Alpha1Recrutment_government(cell, taskInfo)
	local newMembersCount = math.random(1, 2)
	SayCompleted(taskInfo.title)
	AcceptRecrutmentParams(cell, taskInfo, newMembersCount, 0.15)
end

function MissionFail_Alpha1Recrutment_government(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayFailed(taskInfo.title)
	cellInfo.fame = cellInfo.fame + 0.3
	if cellInfo.fame > 1.0 then cellInfo.fame = 1.0 end
	TryToStartInvestigation(cell)
end

function MissionAbort_Alpha1Recrutment_government(cell, taskInfo)
	local cellInfo = cell:getInfo()
	cellInfo.fame = cellInfo.fame + 0.3
	if cellInfo.fame > 1.0 then cellInfo.fame = 1.0 end
end

function MissionSuccess_Alpha1Recrutment_military(cell, taskInfo)
	local newMembersCount = math.random(1, 2)
	SayCompleted(taskInfo.title)
	AcceptRecrutmentParams(cell, taskInfo, newMembersCount, 0.25)
end

function MissionFail_Alpha1Recrutment_military(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayFailed(taskInfo.title)
	cellInfo.fame = cellInfo.fame + 0.5
	if cellInfo.fame > 1.0 then cellInfo.fame = 1.0 end
	TryToStartInvestigation(cell)
end

function MissionAbort_Alpha1Recrutment_military(cell, taskInfo)
	local cellInfo = cell:getInfo()
	cellInfo.fame = cellInfo.fame + 0.5
	if cellInfo.fame > 1.0 then cellInfo.fame = 1.0 end
end

function MissionSuccess_Alpha1DrugsTrading(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayCompleted(taskInfo.title)
	cellInfo.cash = cellInfo.cash + math.random(100, 150) * 1000
	cellInfo.fame = cellInfo.fame + 0.3
	if cellInfo.fame > 1.0 then cellInfo.fame = 1.0 end
	local newMorale = taskInfo.morale
	cellInfo.morale = cellInfo.morale + (newMorale - taskInfo.morale) * 0.3
end

function MissionFail_Alpha1DrugsTrading(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayFailed(taskInfo.title)
	cellInfo.fame = cellInfo.fame + 0.5
	if cellInfo.fame > 1.0 then cellInfo.fame = 1.0 end
	local newMorale = cellInfo.morale
	cellInfo.morale = cellInfo.morale + (newMorale - cellInfo.morale) * 0.3
	TryToStartInvestigation(cell)
end

function MissionAbort_Alpha1DrugsTrading(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayFailed(taskInfo.title)
	cellInfo.fame = cellInfo.fame + 0.5
	if cellInfo.fame > 1.0 then cellInfo.fame = 1.0 end
	local newMorale = cellInfo.morale
	cellInfo.morale = cellInfo.morale + (newMorale - cellInfo.morale) * 0.3
end

function MissionSuccess_Alpha1Autostealings(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayCompleted(taskInfo.title)
	cellInfo.cash = cellInfo.cash + math.random(18, 22) * 1000
	cellInfo.fame = cellInfo.fame + 0.2
	if cellInfo.fame > 1.0 then cellInfo.fame = 1.0 end
	local newMorale = taskInfo.morale
	cellInfo.morale = cellInfo.morale + (newMorale - cellInfo.morale) * 0.25
end

function MissionFail_Alpha1Autostealings(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayFailed(taskInfo.title)
	cellInfo.fame = cellInfo.fame + 0.4
	if cellInfo.fame > 1.0 then cellInfo.fame = 1.0 end
	local newMorale = taskInfo.morale
	cellInfo.morale = cellInfo.morale + (newMorale - cellInfo.morale) * 0.4
	TryToStartInvestigation(cell)
end

function MissionAbort_Alpha1Autostealings(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayFailed(taskInfo.title)
	cellInfo.fame = cellInfo.fame + 0.4
	if cellInfo.fame > 1.0 then cellInfo.fame = 1.0 end
	local newMorale = taskInfo.morale
	cellInfo.morale = cellInfo.morale + (newMorale - cellInfo.morale) * 0.4
end

function MissionSuccess_Alpha1Freelance(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayCompleted(taskInfo.title)
	cellInfo.cash = cellInfo.cash + math.random(3, 7) * 1000
	cellInfo.fame = cellInfo.fame + 0.05
	if cellInfo.fame > 1.0 then cellInfo.fame = 1.0 end
	local newMorale = 1.0
	cellInfo.morale = cellInfo.morale + (newMorale - cellInfo.morale) * 0.1
end

function MissionFail_Alpha1Freelance(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayFailed(taskInfo.title)
	local newMorale = 0.0
	cellInfo.morale = cellInfo.morale + (newMorale - cellInfo.morale) * 0.05
end

function MissionAbort_Alpha1Freelance(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayFailed(taskInfo.title)
	local newMorale = 0.0
	cellInfo.morale = cellInfo.morale + (newMorale - cellInfo.morale) * 0.05
end

function MissionSuccess_Alpha1PrivateDetectiveInvestigation(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayCompleted(taskInfo.title)
	cellInfo.cash = cellInfo.cash + math.random(9, 12) * 1000
	cellInfo.fame = cellInfo.fame + 0.1
	if cellInfo.fame > 1.0 then cellInfo.fame = 1.0 end
	local newMorale = 0.9
	cellInfo.morale = cellInfo.morale + (newMorale - cellInfo.morale) * 0.25
end

function MissionFail_Alpha1PrivateDetectiveInvestigation(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayFailed(taskInfo.title)
	local newMorale = 0.0
	cellInfo.morale = cellInfo.morale + (newMorale - cellInfo.morale) * 0.05
end

function MissionAbort_Alpha1PrivateDetectiveInvestigation(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayFailed(taskInfo.title)
	local newMorale = 0.0
	cellInfo.morale = cellInfo.morale + (newMorale - cellInfo.morale) * 0.05
end
