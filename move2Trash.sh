#!/bin/bash
set -o nounset # set -u (exits, if you try to use an uninitialised variable)
set -o errexit # set -e (exits, if any statement returns a non-true value)

# This script will alias 'rm' in such a way that when a file/folder is deleted from command line, it will be moved into Trash. These files can also be restored to their original path. This is equivalent to deleting files in GUI. This script even works with multiples files and/or folders.

alias rm='move2Trash'

move2Trash()
{
	TRASH_DIR=$HOME/.local/share/Trash/
	while [ ! -z "$1" ]; do
		file_name=`echo "$1"|awk -F/ '{print $NF}'`
		
		#----- This is to avoid file overwriting(by renaming) -------
		file_base=${file_name%.*}
		file_ext=""
		if [ ! "$file_name" == "$file_base" ]; then
		   file_ext="."${file_name##*.}
		fi
		count=1;
		# check whether the file, with similar name, exists in Trash
		#       and if exists, move to Trash with a new name.
		while [ -z "" ]; do
		   if [ ! -e $TRASH_DIR/files/$file_name ]; then
		      break
		   fi
		   count=`expr $count + 1`
		   file_name=$file_base.$count$file_ext
		done
		#------------------------------------------------------------
		
		# extract the complete file name (with path) and moving the file to Trash
		full_file_name=`echo $(cd $(dirname "$1") && pwd)/$(basename "$1")`
		mv "$1" $TRASH_DIR/files/$file_name
		# if 'mv' operation is successful, log file info for restoring 
		if [ $? == 0 ]; then
			echo '[Trash Info]' >$TRASH_DIR/info/$file_name.trashinfo
			echo "Path=$full_file_name" >>$TRASH_DIR/info/$file_name.trashinfo
			echo 'DeletionDate='`date +%F`'T'`date +%T` >>$TRASH_DIR/info/$file_name.trashinfo
		fi
		#continue for all arguments
		shift
	done
}

#add this code to "~/.bashrc" file and execute "source ~/.bashrc".

#********** End ***************
