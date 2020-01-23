#!/bin/bash

#case 0
function options()
{
  echo "------------------------------------------------------------------------------"
  echo "Choose your choice: "
  echo "1 -> initialize before a new grading ..."
  echo "2 -> everytime after run \"dungeon --save\", check if dungeon file unpdate ..."
#  echo "3 -> backup the dungeon file"
  echo "3 -> move \"hello dungeon\" to rlg327 folder"
  echo "0 -> exit"
  read val
  echo "$STR"
}

#case 1: initialize before a new grading
function inil()
{
  echo "** START **: initialization"
  while [ -f ~/.rlg327/dungeon ]
  do
    rm ~/.rlg327/dungeon
    echo "rm ~/.rlg327/dungeon"
  done
  
  if [ ! -f ~/.rlg327/dungeon ];then
    echo "** FINISHED **: initialization"
    echo "$STR"
    Secho "$STR"
  fi
  
  
}

#case 2: check dungeon's md5 and timestamp
function check_update()
{
  echo "** START **: checking md5 and timestamp of dungeon"
  if [ -f ~/.rlg327/dungeon ];then
    echo "Dungeon md5sum: "
    cd ~/.rlg327
    md5sum dungeon
    echo "Dungeon timestamp: "
    cd ~/.rlg327
    ls -la --time-style=full-iso dungeon
    echo "$STR"
  else 
    echo "File dungeon DOES NOT exist in path .rlg327"
    echo "$STR"
  fi
  
}


#case 3 mv hello.rlg327 to dungeon
function movehello()
{
  if [ -f ~/test_dungeon_files/hello.rlg327 ];then
    echo "cp ~/test_dungeon_files/hello.rlg327 ~/.rlg327/dungeon"
    cp ~/test_dungeon_files/hello.rlg327 ~/.rlg327/dungeon
  fi
  if [ -f ~/.rlg327/dungeon ];then
    echo "** FINISHED ** copy hello dungeon"
    echo "$STR"
  fi

}


#START
options


while [ "$val" -ne "0" ]
do
  case $val in
    1)
    inil
    options
    ;;
    2)
    check_update
    options
    ;;
    4)
    movehello
    options
    ;;

  esac
done



exit 0