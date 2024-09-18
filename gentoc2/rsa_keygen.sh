#!/bin/sh
OUT_DIR=./keys
RSA_PRIV_NAME=rsa_private_generated.txt
RSA_PUBLIC_NAME=rsa_public_generated.txt
RSA_KEY_LEN=2048
SIGN_KEY_FILE_RSA=amcu_private_key
MOD_NAME=rsa_to_c_generated.txt
RSA_TO_C_PYTHON_SCRIPT=./rsa_to_c.py

# Generate the RSA-2048 public and private key

rm -f $OUT_DIR/$RSA_PRIV_NAME 
rm -f $OUT_DIR/$RSA_PUBLIC_NAME 
rm -f $OUT_DIR/$MOD_NAME 
rm -f $OUT_DIR/$SIGN_KEY_FILE_RSA.pem
openssl genrsa -out $OUT_DIR/$RSA_PRIV_NAME $RSA_KEY_LEN
openssl rsa -in $OUT_DIR/$RSA_PRIV_NAME -outform PEM -pubout -out $OUT_DIR/$RSA_PUBLIC_NAME

#         # Create C-code ready public key
python3 $RSA_TO_C_PYTHON_SCRIPT $OUT_DIR/$RSA_PUBLIC_NAME > $OUT_DIR/$MOD_NAME

#         # Copy the contents of the generated private key into the .pem file
cp $OUT_DIR/$RSA_PRIV_NAME $OUT_DIR/$SIGN_KEY_FILE_RSA.pem


