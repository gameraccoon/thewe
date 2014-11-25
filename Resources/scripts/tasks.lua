-- Определяем, считать задание выполненным или провеленным
function CheckTaskStatus(cellInfo, taskInfo)
	local successChance = CalcTaskSuccessChance(cellInfo, taskInfo)
	return math.random(0, 1000) / 1000.0 < successChance
end

-- возвращает вероятность выполнения конкретного задания конкретной ячейкой
function CalcTaskSuccessChance(cellInfo, taskInfo)
	local successChance = 1 - math.abs(cellInfo.morale - taskInfo.morale)
	successChance = successChance * (0.5 + cellInfo.devotion * 0.5)

	successChance = successChance * math.pow(1.3, LevelFromExperience(cellInfo.experience) - taskInfo.level)

	if successChance > 1.0 then successChance = 1.0 end
	if successChance < 0.0 then successChance = 0.0 end
	return successChance
end

-- определяем будет ли показываться задание для данной ячейки
function IsShowTaskInList(cell, taskInfo)
	local cellInfo = cell:getInfo()

	-- задачи для туториала
	if World:isTutorialStateAvailable("StartFirstTask") then
		if taskInfo.id == "tutorial_Recrutment" then
			return true
		else
			return false
		end
	elseif taskInfo.id == "tutorial_Recrutment" then
		return false
	end

	if World:isTutorialStateAvailable("ReadyToFirstRealWork") then
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
	local newDevotion = cellInfo.devotion * 2 * (1 - math.abs(cellInfo.morale - newMorale))
	if newDevotion > 1.0 then newDevotion = 1.0 end
	return newMorale, newDevotion
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

-- отправляем игроку сообщение, о том что задание выполнено
function SayCompleted(taskName)
	MessageManager:sendMessage("Задание выполнено\n" .. taskName)
end

-- отправляем игроку сообщение, о том что задание провалено
function SayFailed(taskName)
	MessageManager:sendMessage("Задание провалено\n" .. taskName)
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

	AddExperience(cellInfo, 5)
end

function MissionSuccess_TutorialFirstRealWork(cell, taskInfo)
	local cellInfo = cell:getInfo()
	SayCompleted(taskInfo.title)

	-- мораль ячейки приближается к морали задания
	cellInfo.morale = cellInfo.morale + (taskInfo.morale - cellInfo.morale) * 0.3

	-- увеличивем преданность на 10% от текущего
	cellInfo.devotion = cellInfo.devotion + (cellInfo.devotion * 0.1)
	if (cellInfo.devotion > 1.0) then cellInfo.devotion = 1.0 end

	cellInfo.cash = cellInfo.cash + math.random(40, 60) * 1000

	World:runTutorialFuncton("AfterRealWorkDone")

	AddExperience(cellInfo, 5)
end

function MissionSuccess_Alpha1Recrutment_university(cell, taskInfo)
	local cellInfo = cell:getInfo()
	local newMembersCount = math.random(2, 4)
	SayCompleted(taskInfo.title)
	AcceptRecrutmentParams(cell, taskInfo, newMembersCount, 0.05)
	AddExperience(cellInfo, 2)
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
	local cellInfo = cell:getInfo()
	local newMembersCount = math.random(1, 2)
	SayCompleted(taskInfo.title)
	AcceptRecrutmentParams(cell, taskInfo, newMembersCount, 0.15)
	AddExperience(cellInfo, 15)
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
	local cellInfo = cell:getInfo()
	local newMembersCount = math.random(1, 2)
	SayCompleted(taskInfo.title)
	AcceptRecrutmentParams(cell, taskInfo, newMembersCount, 0.25)
	AddExperience(cellInfo, 350)
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
	AddExperience(cellInfo, 300)
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
	AddExperience(cellInfo, 25)
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
	AddExperience(cellInfo, 3)
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
	AddExperience(cellInfo, 5)
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
