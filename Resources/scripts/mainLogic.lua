local StartWithoutTutorial = false

function StartGame()
	if World:isFirstLaunch() then
		FirstLaunch()
	elseif World:getTutorialState() ~= "TutorialsEnd" then
		if not StartWithoutTutorial then
			ContinueTutorial()
		end
	end
end

function FirstLaunch()
	if not StartWithoutTutorial then
		RunTutorial_Welcome()
	end
end
