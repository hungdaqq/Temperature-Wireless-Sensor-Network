import time
import gspread
from oauth2client.service_account import ServiceAccountCredentials
import json
# open terminal logging
file = open("terminal_log.txt", "r")
# log Google Cloud service
gc = gspread.service_account("wsn-log-378209-4c11a2ffc071.json")
# open file
wks = gc.open("WSN-log")
oldDate = 'j cx dc'
# sh = wks.worksheet(oldDate)
# i = 0
while 1:
    where = file.tell()
    line = file.readline()
    if not line:
        time.sleep(1)
        file.seek(where)
    else:
        array = line.split(',')
        # extract comming event
        extractEvent = array[0].split('"')
        getEvent = extractEvent[len(extractEvent)-2]
        if (getEvent == "receive"): 
            extractTime = array[2]
            extractTime = extractTime.split('"')
            extractTime = extractTime[3].split('T')
            # get date and time
            getDate = extractTime[0]
            getTime = extractTime[1]
            # extract nodeId
            extractNodeId = array[3]
            extractNodeId = extractNodeId.split(':')
            getNodeId = extractNodeId[-1]
            # extract message
            extractMsg = array[-1]
            extractMsg = extractMsg.split('"')
            getMsg = extractMsg[3]
            # create a new sheet for new day
            if (oldDate != getDate):
                wks.add_worksheet(title=getDate, rows="5000", cols="20")
                sh = wks.worksheet(getDate)
                i = 0
            sh.update_cell(i+1,1,getTime)
            sh.update_cell(i+1,2,getNodeId)
            sh.update_cell(i+1,3,getMsg)
            i += 1
            oldDate = getDate

# # Update a range of cells using the top left corner address
# wks.update('A1', [[1, 2], [3, 4]])

# # Or update a single cell
# wks.update('B42', "it's down there somewhere, let me take another look.")

# # Format the header
# wks.format('A1:B1', {'textFormat': {'bold': True}})