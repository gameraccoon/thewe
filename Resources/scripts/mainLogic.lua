local StartWithoutTutorial = false

-- кешированные значения опыта для достижения уровней
local CashedTableOfLevels = {}

-- запуск игры после загрузки ресурсов
function StartGame()
	if World:isFirstLaunch() then
		FirstLaunch()
	elseif World:getTutorialState() ~= "TutorialsEnd" then
		if not StartWithoutTutorial then
			ContinueTutorial()
		end
	end
end

-- логика, которая выполняется только при первом запуске игры
function FirstLaunch()
	if not StartWithoutTutorial then
		RunTutorial_Welcome()
	end
end

-- вычисляем уровень ячейки из опыта
function LevelFromExperience(experience)
	local nextLevelExp = 0
	local nextLevel = 0
	while experience < nextLevelExp do
		nextLevel = nextLevel + 1
		nextLevelExp = ExperienceForLevel(nextLevel)
	end
	return nextLevel - 1
end

-- вычисляем сколько опыта нужно для достижения определённого уровня
function ExperienceForLevel(level)
	-- ищем значение в кеше
	local expValue = CashedTableOfLevels[level]

	-- если значение не нашли
	if expValue == nil then
		-- вычисляем значение
		expValue = math.pow(math.exp(level), 0.3)
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
