#!/bin/bash

UPDATE_MODE='N'
VALID_NUM='^[0-9]+$'
HELP_MODE='N'
INFILE=""
DELIMITER=',' # default delimiter is comma
UPDATE_MODE='N'
NAMECOL=1
USERCOL=3
ENCRYPTCOL=4

for i in "$@"
do
case $i in
	-d=*|--delimiter=*)
    delimiter="${i#*=}"
    shift # past argument=value
    ;;

	-f=*|--fullname_column=*)
	if [[ $"${i#*=}" =~ $VALID_NUM ]]; then
		NAMECOL="${i#*=}"
	fi
	shift # past argument=value
	;;

    -i=*|--input_file=*)
    INFILE="${i#*=}"
    shift # past argument=value
    ;;

    -h|--help)
    HELP_MODE='Y'
    shift # to next argument
    ;;

	-u|--update)
    UPDATE_MODE='Y'
    shift # to next argument
    ;;

    -y=*|--username_column=*)
	if [[ $"${i#*=}" =~ $VALID_NUM ]]; then
		USERCOL="${i#*=}"
	fi
	shift # past argument=value
	;;

    -z=*|--encrypted_column=*)
	if [[ $"${i#*=}" =~ $VALID_NUM ]]; then
		ENCRYPTCOL="${i#*=}"
	fi
	shift # past argument=value
	;;

    *)
            # unknown option
    ;;
esac
done

echo "Input file: $INFILE"
echo "Delimiter: $DELIMITER"
echo "Full name column: $NAMECOL"
echo "Username column: $USERCOL"
echo "Encrypted column: $ENCRYPTCOL"
echo "Update mode: $UPDATE_MODE"
echo ""

if [[ "$UPDATE_MODE" = "Y" ]]; then
	cat "$INFILE" | awk -F $DELIMITER -v user_col=$USERCOL -v name_col=$NAMECOL '{print "id -u "$user_col" &>/dev/null || sudo /usr/sbin/useradd -s /bin/bash -m -G users -c \""$name_col"\" "$user_col}' | xargs -0 bash -c
	cat "$INFILE" | awk -F $DELIMITER -v user_col=$USERCOL -v encrypt_col=$ENCRYPTCOL '{print "sudo usermod -p "$encrypt_col" "$user_col}' | xargs -0 bash -c
else
	cat "$INFILE" | awk -F $DELIMITER -v user_col=$USERCOL '{print "Creating and/or updating password for user "$user_col}'
fi