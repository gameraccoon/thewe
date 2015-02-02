-- Определяем, считать задание выполненным или провеленным
function CheckTaskStatus(cell, taskInfo)
	local successChance = CalcTaskSuccessChance(cell, taskInfo)
	return math.random(0, 1000) / 1000.0 < successChance
end

-- возвращает вероятность выполнения конкретного задания конкретной ячейкой
function CalcTaskSuccessChance(cell, taskInfo)
	local successChance = 1

	successChance = successChance * math.pow(1.3, LevelFromExperience(cell.experience) - taskInfo.level)

	if successChance > 1.0 then successChance = 1.0 end
	if successChance < 0.0 then successChance = 0.0 end
	return successChance
end

-- определяем будет ли показываться задание для данной ячейки
function IsShowTaskInList(cell, taskInfo)
	-- задачи для туториала
	if World:getTutorialManager():isTutorialStateAvailable("StartFirstTask") then
		if taskInfo.id == "tutorial_Recrutment" then
			return true
		else
			return false
		end
	elseif taskInfo.id == "tutorial_Recrutment" then
		return false
	end

	if World:getTutorialManager():isTutorialStateAvailable("ReadyToFinishFirstRealWork") then
		if taskInfo.id == "tutorial_RealWork" then
			return true
		else
			return false
		end
	elseif taskInfo.id == "tutorial_RealWork" then
		return false
	end

	return true
end

function MissionSuccess_InvestigatorTest(cell, taskInfo)
	MessageManager:sendMessage("Investigation launched")
	World:addInvestigatorByCellUid(cell:getUid())
end