local StartWithoutTutorial = false

-- кешированные значения опыта для достижения уровней
local CashedTableOfLevels = {}

-- запуск игры после загрузки ресурсов
function StartGame()
	if World:isFirstLaunch() then
		FirstLaunch()
	elseif World:getTutorialManager():isTutorialStateAvailable("TutorialsEnd") then
		ContinueTutorial()
	end
end

-- логика, которая выполняется только при первом запуске игры
function FirstLaunch()
	if not StartWithoutTutorial then
		RunTutorial_Welcome()
	end
end

-- вызывается когда игрок подбирает бонус
function BonusBehavior(cell)
	local cellInfo = cell:getInfo()
	cellInfo.cash = cellInfo.cash + math.random(1, 20) * 1000
	cellInfo.morale = cellInfo.morale + cellInfo.morale * 0.2
	cellInfo.experience = cellInfo.experience + cellInfo.devotion * 0.1
end

-- вычисляем уровень ячейки из опыта
function LevelFromExperience(experience)
	local nextLevelExp
	local nextLevel = 1

	repeat
		nextLevel = nextLevel + 1
		nextLevelExp = ExperienceForLevel(nextLevel)
	until (experience < nextLevelExp)

	return nextLevel - 1
end

-- вычисляем сколько опыта нужно для достижения определённого уровня
function ExperienceForLevel(level)
	-- ищем значение в кеше
	local expValue = CashedTableOfLevels[level]

	-- если значение не нашли
	if expValue == nil then
		-- вычисляем значение
		expValue = math.floor(math.exp(level)) - 2
		-- кешируем
		CashedTableOfLevels[level] = expValue
	end
	
	return expValue
end

-- вычисляем уровень преследуемости
function CalcCellPursuedLevel(cell)
	local cellInfo = cell:getInfo()
	return cellInfo.fame * (1 - cellInfo.townInfluence)
end

-- вычисляем вероятность, с которой у данной ячейки может появиться инвестигейтор
function GetInvestigationChance(cell)
	local connectivity = cell:calcConnectivity()
	local distanceToRoot = cell:calcDistanceToTheRootCell()
	local pursuedLevel = CalcCellPursuedLevel(cell)

	Log:log("conn" .. connectivity)

	local investigationChance = pursuedLevel

	if distanceToRoot < 2 then
		investigationChance = investigationChance * 0.3
	end

	if distanceToRoot == 0 then
		investigationChance = 0
	end

	investigationChance = investigationChance * math.min(connectivity / 5.0, 1.0)

	return investigationChance
end

function AddExperience(cell, expAmount)
	cell:setExp(cell:getExp() + expAmount)
end