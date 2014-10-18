local StartWithoutTutorial = false

function StartGame()
	if World:isFirstLaunch() then
		FirstLaunch()
	end
end

function FirstLaunch()
	if not StartWithoutTutorial then
		RunTutorial_Welcome()
	end
end
