<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
        <meta name="description" content="">
        <meta name="author" content="">
        <link rel="icon" href="img/favicon.ico">
        <!-- WEBPAGE TITLE -->
        <title>Me-MG Dashboard</title>
        
        <!-- Latest compiled and minified CSS -->

        <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
        
        <script src="js/jquery-ui.min.js"></script>

        
        <script src="//maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
        
        <script src="https://cdnjs.cloudflare.com/ajax/libs/tether/1.4.0/js/tether.min.js"></script>
        
        <script src="//cdnjs.cloudflare.com/ajax/libs/dygraph/2.1.0/dygraph.min.js"></script>
        <link rel="stylesheet" href="//cdnjs.cloudflare.com/ajax/libs/dygraph/2.1.0/dygraph.min.css" />
        <link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css">
    	
        <script src="js/azure-storage.blob.min.js"></script>
        <script src="js/dygraph.js"></script>
        
		
        <link rel="stylesheet" href="//maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
        
        <link href="https://fonts.googleapis.com/css?family=Montserrat" rel="stylesheet">
        <link href="https://fonts.googleapis.com/css?family=Montserrat+Alternates" rel="stylesheet">
        
        <!-- CSS OVERWRITES -->
    	
        <link href="css/navbar.css" rel="stylesheet">
      	<link href="css/header.css" rel="stylesheet">
        <link href="css/button.css" rel="stylesheet">
    		
      	<link href="css/dashboard.css" rel="stylesheet">
      	
      	<link href="css/colours/colours.css" rel="stylesheet">

    </head>
    
    <body>
    	<!-- NAVBAR -->
    	<nav class="navbar navbar-inverse navbar-fixed-top">
	      <div class="container">
	        <div class="navbar-header">
            <!-- NAVBAR - HAMBURGER BUTTON -->
	          <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#navbar" aria-expanded="false" aria-controls="navbar">
	            <span class="sr-only">Toggle navigation</span>
	            <span class="icon-bar"></span>
	            <span class="icon-bar"></span>
	            <span class="icon-bar"></span>
	          </button>
	          
	          <!-- NAVBAR - PROJECT NAME-->
	          <a class="navbar-brand" href="/"> 
              Me-MG
            </a>
            <img src="img/MeMG_Logo1.png" style="width: 50px; height: auto; vertical-align: middle; margin-left: -10px; padding: 8px;" class="navbar-brand"/>
	        </div>
	        
	        <div id="navbar" class="navbar-collapse collapse">
	          <span class="navbar-form navbar-right" style="    color: white;
				padding: inherit;
				font-size: x-large;"> Welcome, Physio! </span>
	        </div><!--/.navbar-collapse -->
	      </div>
	    </nav>
    

      <!-- MAIN CONTENT - COLUMNS -->

      <div class="jumbotron noCarousel">
        <div class="container">
          <!-- MAIN CONTENT - HEADER -->
          <h1>Me-MG Dashboard</h1>
          
          <p>
            Current Device Selected:
              <select id = "deviceSelect" onchange="" size = "1" style="height: 30px; line-height: 30px; margin: 10px;">
                <option value="1">Device#101</option>
                <option value="2">Device#102</option>
                <option value="3">Device#103</option>
                <option value="3">Device#104</option>
              </select>
          </p>
          
          
          
          <div class="row" style="text-align: center; margin-top:10px;">
            <div class="col-md-4 ">
              <a class="btn btn-primary btn-lg blueberry dashboardBtns" href="#" role="button">Set Extension Threshold</a>
            </div>
            
            <div class="col-md-4 ">
              <a class="btn btn-primary btn-lg blueberry dashboardBtns" href="#" role="button">Set Muscle Activation Threshold</a>
            </div>
            
            <div class="col-md-2 ">
              <a class="btn btn-primary btn-lg blueberry dashboardBtns" href="#" role="button">Edit Goals</a>
            </div>
            <div class="col-md-2 ">
              <a class="btn btn-primary btn-lg blueberry dashboardBtns" href="#" role="button" onclick="toggleCompare()">Compare</a>
            </div>
          </p>
        </div>
      </div>
	  
	  <div id = "compareTable" class="w3-container" style="position:absolute; right:235px; display: none;">
		<table class="w3-table-all w3-card-4">
			<tr>
				<th> </th>
				<th> This Session </th>
				<th> Last Session </th>
			</tr>
			<tr>
				<th> Reps </th>
				<th> 8 </th>
				<th> 5 </th>
			</tr>
			<tr>
				<th> Max EMG </th>
				<th> 800 </th>
				<th> 500 </th>
			</tr>
		</table>
	</div>
				
	  
      
      <div class="container">
        <h1>Muscle Activity</h1>
        <hr/>
        <div class="row">
          <div class="col-md-4" style="height: 400px; overflow: auto;">
            <table id="sessionsTable" class="
                table table-sm table-striped table-bordered table-hover table-responsive" >
                
              <thead>
                <tr>
                  <th scope="col">#</th>
                  <th scope="col">Session Name</th>
                  <th scope="col">#Reps</th>
                  <th scope="col">Display</th>
                </tr>
              </thead>
              <div>
              <tbody id="sessionsBody" style="">
              
              </tbody>
                  
            </table>
          </div>
          <div class="container" style="margin-right:10px;">
            <div id="div_g" class="col-md-8" style="min-width: 300px; max-width: 1200px; height: 400px; ">
            </div>
          </div>
        </div>
        
        <hr/>
      </div>
      
      <!--
      <div class = "container">
        <form>
          <select id = "mySelect" onchange="refreshGraph()" size = "1" style="height: 30px; line-height: 30px; margin: 10px;">
          </select>
        </form>
      </div>
      -->
      
      
      
      <div class="container">
        <h1>Patient's Progress</h1>
        <hr/>
        <div id="div_Progress" style="min-width: 300px; max-width: 1200px; height: 400px; "></div>
      </div>
      
      <div class="container">
        <hr>
        <!-- FOOTER - NAMES AND COMPANY DETAILS -->
        <footer>
          <p>&copy; 2018 - Telstra Challenge - Aaron Duivenvoorden, Steven Korevaar, Timothy Scott, and Jeremy Siaw</p>
        </footer>
      </div>

    </body>
</html>
