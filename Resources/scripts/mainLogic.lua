
function StartGame()
	if World:isFirstLaunch() then
		FirstLaunch()
	end
end

function FirstLaunch()
	AddFirstTutorial()
end

function AddFirstTutorial()
	-- ToDo: make localizations
	local firstTutorialText1 =
		"Hi there!\n"..
		"Press the button below to continue"

	local firstTutorialText2 =
		"This is an example of the tutorial"

	World:addTutorial(Tutorial(firstTutorialText1, "Continue"))
	World:addTutorial(Tutorial(firstTutorialText2, "I've got it"))
end
