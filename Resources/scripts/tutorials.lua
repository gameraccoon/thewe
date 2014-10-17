local ContinueText = "Продолжить"

function RunTutorialWelcome()
	-- ToDo: make localizations
	local text1 =
		"Добро пожаловать в игру!\n"..
		"Нажмите клавишу внизу чтобы продолжить"

	local text2 =
		"Для начала нажмите на любой город,\n"..
		"чтобы основать там свою первую ячейку"

	World:addTutorial(Tutorial(text1, ContinueText))
	World:addTutorial(Tutorial(text2, ContinueText))

	World:setTutorialState("FirstCell");
end

function RunTutorialAfterCreatingFirstCell()
	local text1 =
		"Отлично!\n\n"..
		"Сейчас в ячейке совсем не много сотрудников.\n"..
		"Для действительно великих дел нам\n"..
		"понадобится больше людей.\n\n"..
		"Нажмите на ячейку и зайдите в меню заданий"

	World:addTutorial(Tutorial(text1, ContinueText))

	World:setTutorialState("StartFirstTask");
end

function RunTutorialAfterStartFirstTask()
	local text1 =
		"Теперь выберите задачу вербовки"

	World:addTutorial(Tutorial(text1, ContinueText, "TutorialsLastTutorial"))
end

function TutorialsLastTutorial()
	World:setTutorialState("TutorialsEnd");
end
