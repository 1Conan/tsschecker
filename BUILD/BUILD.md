

TODO: make this buildinstruction look nice  

Step 1) build docker container 
run "docker build -t Xcross ." 

Step 2) start container 
run "docker run --rm -it Xcross bash" 

Step 3) login as user jenkins 
inside the docker container run "su -l jenkins" 

Step 4) copy and paste build.txt into the container shell 

Step 5) wait for it to compile 

Step 6) profit! 
