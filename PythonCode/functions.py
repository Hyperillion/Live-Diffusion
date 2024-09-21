import time

def spaceToUnderscore(string):
	return string.replace(' ', '_')

def commaToUnderscore(string):
	return string.replace(':', '_')

def saveImage():
	# print('savefile pressed')
	# userName = '{}'.format(op('username').par.text)
	PromptString = '{}'.format(op('prompt').par.text)
	timeStruct = time.localtime()
	currentTime = time.strftime('%Y-%m-%d_%H:%M:%S', timeStruct)
	# print('{}'.format(currentTime) )
	# print('{}-{}-{}.png' .format(userName, spaceToUnderscore(PromptString), currentTime))

	saveimage_node = op('imageSave')
	# Set the file path and name
	# print('D:/Install_Location/MangLab/ImageSave/{}-{}-{}.jpg'.format(userName, spaceToUnderscore(PromptString), commaToUnderscore(currentTime)))
	saveLocation = './ImageSave/{}-{}.png'.format(spaceToUnderscore(PromptString), commaToUnderscore(currentTime))
	saveimage_node.par.file = saveLocation
	notify('Image saved to {}'.format(saveLocation))
	#turn on image record
	saveimage_node.par.record = True
	return

def saveEnd():
	op('imageSave').par.record = False
	op('function').module.buttonBlack('buttonSave')
	return

def preset1():
	# op('workflowAPI').par.file = "D:\Install_Location\project_touchdesigner\comfyUI_416\preset1.json"   #for different json file
	# op('username').par.text = "jinran"
	op('prompt').par.text = "cyberpunk neon world"
	op('Stable_Diffusion').par.Node15par9 = 'crop1'
	send()
	# op('Stable_Diffusion').par.Node6par6 = op('prompt').par.text
	# op('Stable_Diffusion').par.Stream = True
	notify('Preset 1 loaded')
	return
	
def preset2():
	# op('workflowAPI').par.file = "D:\Install_Location\project_touchdesigner\comfyUI_416\preset2.json"   #for different json file
	# op('username').par.text = "chenxuan"
	op('prompt').par.text = "in space station"
	op('Stable_Diffusion').par.Node15par9 = 'crop1'
	send()
	notify('Preset 2 loaded')
	# op('Stable_Diffusion').par.Stream = True
	return
	
def stream():
	op('Stable_Diffusion').par.Node15par9 = 'crop1'
	# op('Stable_Diffusion').par.Stream = True
	if (op('Stable_Diffusion').par.Stream):
		op('Stable_Diffusion').par.Stream = False
		buttonBlack('stream1')
		notify('Stream ended')
	else:
		op('Stable_Diffusion').par.Reinit.pulse()
		op('Stable_Diffusion').par.Stream = True
		buttonWhite('stream1')
		notify('Stream started')
	return

def send():
	# op('Stable_Diffusion').par.Node3par2 = int(op('CFG').par.text)
	
	currentPrompt = '{}'.format(op('Stable_Diffusion').par.Node6par6)
	newPrompt = '{}'.format(op('prompt').par.text)
	# print(currentPrompt)
	# print(newPrompt)
	# print(currentPrompt == newPrompt)
	if currentPrompt == newPrompt:
		notify('Same prompt as current, maybe try another one')
		return
	else:
		op('Stable_Diffusion').par.Node6par6 = op('prompt').par.text
		updatePromptHistory(op('prompt').par.text)
		notify('Prompt sent successfully')
		return
	
def buttonBlack(button):
	op(button).par.colorr = 0
	op(button).par.colorg = 0
	op(button).par.colorb = 0
	return

def buttonWhite(button):
	op(button).par.colorr = 1
	op(button).par.colorg = 1
	op(button).par.colorb = 1
	return

def updatePromptHistory(newPrompt = ''):
	if newPrompt != '': #received new prompt
		promptFile = open("previousPrompt.txt", "a")
		# lastPrompt = promptFile.read().splitlines()[-1]
		promptFile.write('{}\n'.format(newPrompt))
		promptFile.close()
	updatedPromptFile = open("previousPrompt.txt", "r")
	promptHistoryList = updatedPromptFile.read().strip("\n")
	op('promptHistory').par.text = promptHistoryList
	# print(promptHistoryList)
	updatedPromptFile.close()
	return

def clearHistory():
	promptFile = open("previousPrompt.txt", "w")
	promptFile.write('\n')
	promptFile.close()
	updatePromptHistory()
	notify('Prompt history cleared')
	return

def notify(notification):
	op('notification').par.text = notification

def testprint():
	print(123)
	return
	
def split_para(message):
	messageList = message.split(',')
	#print(message)
	return messageList

def linear_mapping(value, a, b, c, d):
    mapped_value = (value - a) * (d - c) / (b - a) + c
    return mapped_value

def execute_list(Mlist):
	#Mlist[0] is the potentiometer value
	#Mlist[1] is the rotary switch value
	#Mlist[2] is the up button value
	#Mlist[3] is the down button value
	#Mlist[4] is the rotary decoder value
	if Mlist[1] == '1':
		send()
		notify('Send button pressed')

	# CFG_value = int(Mlist[4])/10
	steps = int(Mlist[4])
	limitedSteps = 10
	if steps <= 1:
		limitedSteps = 1
	elif steps >= 30:
		limitedSteps = 30
	else:
		limitedSteps = steps
	op('Quality1').par.text = limitedSteps
	# op('Stable_Diffusion').par.Node3par2 = CFG_value

	# steps_value = int(Mlist[4])
	# op('steps').par.text = steps_value
	# op('Stable_Diffusion').par.Node3par1 = steps_value

	# denoise_value = int(Mlist[0])/10
	# # denoise_value = linear_mapping(denoise_value, 0, 128, 0, 1)
	# op('denoise').par.text = denoise_value
	# op('Stable_Diffusion').par.Node3par5 = denoise_value


	# strength_value = int(Mlist[0])/100
	# # denoise_value = linear_mapping(denoise_value, 0, 128, 0, 1)
	# op('strength').par.text = strength_value
	# op('Stable_Diffusion').par.Node18par10 = strength_value


if __name__ == "__main__":
	updatePromptHistory('test')
	
	
