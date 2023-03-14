# Image Flipping from an SQL database using ImageMagick

Automate the flipping of a series of images stored in an SQL table using ImageMagick on Ubuntu in Bash:

1.	Open a terminal on your Ubuntu system.
2.	Navigate to the directory where you want to store the flipped images.
3.	Open a text editor and create a new file named "flip_images.sh".
4.	Add the following code to the "flip_images.sh" file:

```sh
#! /usr/bin/env bash
# Connect to the SQL database
mysql -u username -p password -D database_name -e "SELECT image_path FROM table_name" | while read -r image_path; do
#Flip the image vertically using ImageMagick
convert "$image_path" -flip "flipped_$image_path"
done
```


5. Replace "username", "password", "database_name", and "table_name" with the appropriate values for your SQL database.
6.	Save and close the "flip_images.sh" file.
7.	Make the file executable by running the following command:

```
chmod +x flip_images.sh
```

8.	Run the script by entering the following command in the terminal:

```
./flip_images.sh
```
