#!/bin/bash

echo Output File Name:
read output_file

echo Remove Pasphere:
read rem_pass

openssl genrsa -des3 -out ./$output_file.key 1024
openssl req -new -key ./$output_file.key -out ./$output_file.csr

if [ "$rem_pass" != "yes" ]
then
	cp ./$output_file.key ./$output_file.key.org
	openssl rsa -in ./$output_file.key.org -out ./$output_file.key
fi

openssl x509 -req -days 365 -in $output_file.csr -signkey $output_file.key -out $output_file.crt

