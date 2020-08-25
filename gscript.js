// BY: Akshaya Niraula
// ON: 2016 November 12th.
// AT: http://www.embedded-lab.com/.....


// Steps are valid as of 2016 November 12th.
// 0) From Google spreadsheet, Tools > Scriipt Editor...
// 1) Write your code
// 2) Save and give a meaningful name
// 3) Run and make sure "doGet" is selected
//    You can set a method from Run menu
// 4) When you run for the first time, it will ask 
//    for the permission. You must allow it.
//    Make sure everything is working as it should.
// 5) From Publish menu > Deploy as Web App...
//    Select a new version everytime it's published
//    Type comments next to the version
//    Execute as: "Me (your email address)"
//    MUST: Select "Anyone, even anonymous" on "Who has access to this script"
//    For the first time it will give you some prompt(s), accept it.
//    You will need the given information (url) later. This doesn't change, ever!

// Saving the published URL helps for later.
// https://script.google.com/macros/s/---Your-Script-ID--Goes-Here---/exec
// https://script.google.com/macros/s/---Your-Script-ID--Goes-Here---/exec?tag=test&value=-1

// This method will be called first or hits first  
function doGet(e){
  Logger.log("--- doGet ---");
  
  var tag = "", 
      value = "";
  
  try {
    
    // this helps during debuggin
    if (e == null){e={}; e.parameters = {tag:"test",value:"-1"};}
    
    tag = e.parameters.tag;
    value = e.parameters.value;
    
    // save the data to spreadsheet
    save_data(tag, value);
    
    
    return ContentService.createTextOutput("Wrote:\n  tag: " + tag + "\n  value: " + value);
    
  } catch(error) { 
    Logger.log(error);    
    return ContentService.createTextOutput("oops...." + error.message 
                                            + "\n" + new Date() 
                                            + "\ntag: " + tag +
                                            + "\nvalue: " + value);
  }  
}

// Method to save given data to a sheet
function save_data(tag, value){
  Logger.log("--- save_data ---"); 
  
  
  try {
    var dateTime = new Date();
    
    // Paste the URL of the Google Sheets starting from https thru /edit
    // For e.g.: https://docs.google.com/..../edit 
    var ss = SpreadsheetApp.openByUrl("https://docs.google.com/---Your-Script-ID--Goes-Here---/edit");
    var dataLoggerSheet = ss.getSheetByName("DataLogger");
    
    
    // Get last edited row from DataLogger sheet
    var row = dataLoggerSheet.getLastRow() + 1;
    
    
    // Start Populating the data
    dataLoggerSheet.getRange("A" + row).setValue(row -1); // ID
    dataLoggerSheet.getRange("B" + row).setValue(dateTime); // dateTime
    dataLoggerSheet.getRange("C" + row).setValue(tag); // tag
    dataLoggerSheet.getRange("D" + row).setValue(value); // value
    
    
  }
  
  catch(error) {
    Logger.log(JSON.stringify(error));
  }
  
  Logger.log("--- save_data end---"); 
}