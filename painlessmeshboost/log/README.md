# Google Spreadsheets Python API v4
Features:
- Open a spreadsheet by title, key or url.
- Read, write, and format cell ranges.
- Sharing and access control.
- Batching updates.

## Installation
``
pip3 install gspread
``

Requirements: Python 3.6+.

## Usage 

1. [Create credentials in Google API Console](https://docs.gspread.org/en/latest/oauth2.html)
2. Run painlessMeshBoost as a client of the mesh. With `<ip>` is the default route. You can set log level as the message you want your client to listen to. For example: `receive`, `change`,... And then log terminial into a `.txt` files.

``
painlessMeshBoost -c <ip> >> terminal_log.txt
``
3. Run `log.py` with the credential and API created from step 1.
``
python3 log.py
``
4. Open the spreadsheet and see the result.
