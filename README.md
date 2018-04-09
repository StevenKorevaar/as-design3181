To Set Up the GIT REPOSITORY

First, get GIT:
https://gitforwindows.org/

Then open GIT Bash and navigate to the folder you want to store all the files and run:
$ git clone https://github.com/StevenKorevaar/as-design3181.git

-----------------------------------------------------------------------------------------
Now to actually Run the Website:

Install this:
https://chocolatey.org/

Then run the commands:

$ choco install php

$ choco upgrade php

Then navigate to the GIT repo and run the command:

$ php -S localhost:8000

Then goto: http://localhost:8000 

---------------------------------------------------------------------------------------
Always Before you start development run:
$ git pull


Once you've made changes:

Run:

$ git pull

$ git add .

$ git commit -m "The changes youve made to the repo"

Once you have FINISHED DEVELOPMENT (ie. there are no website crashing or failing errors) 
this is done so that your development does not get in the way of anyone elses. Then you can run: 

$ git push

---------------------------------------------------------------------------------------
To commit the latest changes to Azure

$ git remote add azure  https://LaikaUser@laikaweb.scm.azurewebsites.net/laikaweb.git (I believe you will only need to do this once)

$ git push azure master

