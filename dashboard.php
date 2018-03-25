<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
        <meta name="description" content="">
        <meta name="author" content="">
        <link rel="icon" href="../../favicon.ico">
        <!-- WEBPAGE TITLE -->
        <title>Engineering Design 3A - Telstra Challenge Back End</title>
        
        <!-- Latest compiled and minified CSS -->
    	<link rel="stylesheet" href="//maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
    		
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
        <script>window.jQuery || document.write('<script src="../../assets/js/vendor/jquery.min.js"><\/script>')</script>
        
        <script src="https://cdnjs.cloudflare.com/ajax/libs/tether/1.4.0/js/tether.min.js" integrity="sha384-DztdAPBWPRXSA/3eYEEUWrWCy7G5KFbe8fFjk5JAIxUYHKkDx6Qin1DkWx51bBrb" crossorigin="anonymous"></script>
        
        <!-- Latest compiled and minified JavaScript -->
        <script src="//maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
    		
    	  <!-- CSS OVERWRITES -->
    	
        <link href="css/navbar.css" rel="stylesheet">
      	<link href="css/header.css" rel="stylesheet">
    	<link href="css/button.css" rel="stylesheet">
    		
      	<link href="css/dashboard.css" rel="stylesheet">
      	
      	<link href="css/colours/darkblue.css" rel="stylesheet">
        
        
        <script src="//cdnjs.cloudflare.com/ajax/libs/dygraph/2.1.0/dygraph.min.js"></script>
        <link rel="stylesheet" href="//cdnjs.cloudflare.com/ajax/libs/dygraph/2.1.0/dygraph.min.css" />
    	
        <script src="js/dygraph.js"></script>
        
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
	          <a class="navbar-brand" href="index.html"> Telstra Challenge</a>
	        </div>
	        
	        <div id="navbar" class="navbar-collapse collapse">
	          <!-- NAVBAR - SIGN IN FORM -->
	          <form class="navbar-form navbar-right">
	            <div class="form-group">
	              <!-- NAVBAR - LOGIN - EMAIL -->
	              <input type="text" placeholder="Email" class="form-control">
	            </div>
	            <div class="form-group">
	              <!-- NAVBAR - LOGIN - PASSWORD -->
	              <input type="password" placeholder="Password" class="form-control">
	            </div>
	            <!-- NAVBAR - LOGIN - SUBMIT BUTTON -->
	            <button type="submit" class="btn btn-signin">Sign in</button>
	          </form>
	        </div><!--/.navbar-collapse -->
	      </div>
	    </nav>
    

    <!-- MAIN CONTENT - COLUMNS -->

		<div class="jumbotron noCarousel">
      <div class="container">
        <!-- MAIN CONTENT - HEADER -->
        <h1>Welcome to the Dashboard!</h1>
        <p>This page is where you can see your devices datalogs and perform any actions that the device can do!</p>
        <p>
          <a class="btn btn-primary btn-lg btn-red" href="#" role="button">Learn more &raquo;</a>
          <a class="btn btn-primary btn-lg blueberry" href="#" role="button">Learn more &raquo;</a>
        </p>
      </div>
    </div>
    
    <div id="div_g" style="min-width: 310px; height: 400px; margin: 0 auto"></div>
    <!--
    <div id="container" style="min-width: 310px; height: 400px; margin: 0 auto"></div>
    -->
    <div class="container">
        <main class="col-sm-9 offset-sm-3 col-md-10 offset-md-2 pt-3">
          <h1>Dashboard</h1>
          <h2>Section title</h2>
          <div class="table-responsive">
            <table class="table table-striped">
              <thead>
                <tr>
                  <th>#</th>
                  <th>Header</th>
                  <th>Header</th>
                  <th>Header</th>
                  <th>Header</th>
                </tr>
              </thead>
              <tbody>
                <tr>
                  <td>1,001</td>
                  <td>Lorem</td>
                  <td>ipsum</td>
                  <td>dolor</td>
                  <td>sit</td>
                </tr>
                <tr>
                  <td>1,002</td>
                  <td>amet</td>
                  <td>consectetur</td>
                  <td>adipiscing</td>
                  <td>elit</td>
                </tr>
                <tr>
                  <td>1,003</td>
                  <td>Integer</td>
                  <td>nec</td>
                  <td>odio</td>
                  <td>Praesent</td>
                </tr>
                <tr>
                  <td>1,003</td>
                  <td>libero</td>
                  <td>Sed</td>
                  <td>cursus</td>
                  <td>ante</td>
                </tr>
                <tr>
                  <td>1,004</td>
                  <td>dapibus</td>
                  <td>diam</td>
                  <td>Sed</td>
                  <td>nisi</td>
                </tr>
                <tr>
                  <td>1,005</td>
                  <td>Nulla</td>
                  <td>quis</td>
                  <td>sem</td>
                  <td>at</td>
                </tr>
                <tr>
                  <td>1,006</td>
                  <td>nibh</td>
                  <td>elementum</td>
                  <td>imperdiet</td>
                  <td>Duis</td>
                </tr>
                <tr>
                  <td>1,007</td>
                  <td>sagittis</td>
                  <td>ipsum</td>
                  <td>Praesent</td>
                  <td>mauris</td>
                </tr>
                <tr>
                  <td>1,008</td>
                  <td>Fusce</td>
                  <td>nec</td>
                  <td>tellus</td>
                  <td>sed</td>
                </tr>
                <tr>
                  <td>1,009</td>
                  <td>augue</td>
                  <td>semper</td>
                  <td>porta</td>
                  <td>Mauris</td>
                </tr>
                <tr>
                  <td>1,010</td>
                  <td>massa</td>
                  <td>Vestibulum</td>
                  <td>lacinia</td>
                  <td>arcu</td>
                </tr>
                <tr>
                  <td>1,011</td>
                  <td>eget</td>
                  <td>nulla</td>
                  <td>Class</td>
                  <td>aptent</td>
                </tr>
                <tr>
                  <td>1,012</td>
                  <td>taciti</td>
                  <td>sociosqu</td>
                  <td>ad</td>
                  <td>litora</td>
                </tr>
                <tr>
                  <td>1,013</td>
                  <td>torquent</td>
                  <td>per</td>
                  <td>conubia</td>
                  <td>nostra</td>
                </tr>
                <tr>
                  <td>1,014</td>
                  <td>per</td>
                  <td>inceptos</td>
                  <td>himenaeos</td>
                  <td>Curabitur</td>
                </tr>
                <tr>
                  <td>1,015</td>
                  <td>sodales</td>
                  <td>ligula</td>
                  <td>in</td>
                  <td>libero</td>
                </tr>
              </tbody>
            </table>
          </div>
        </main>
      </div>

      
      <div class="container">
        <hr>
        <!-- FOOTER - NAMES AND COMPANY DETAILS -->
        <footer>
          <p>&copy; 2018 - Telstra Challenge - Aaron Duivenvoorden, Steven Korevaar, Timothy Scott, and Leanne Tran</p>
        </footer>
      </div>

    </body>
</html>
