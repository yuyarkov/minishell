RESET="\033[0m"
BLACK="\033[30m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
BLUE="\033[34m"
MAGENTA="\033[35m"
CYAN="\033[36m"
WHITE="\033[37m"

BOLDBLACK="\033[1m\033[30m"
BOLDRED="\033[1m\033[31m"
BOLDGREEN="\033[1m\033[32m"
BOLDYELLOW="\033[1m\033[33m"
BOLDBLUE="\033[1m\033[34m"
BOLDMAGENTA="\033[1m\033[35m"
BOLDCYAN="\033[1m\033[36m"
BOLDWHITE="\033[1m\033[37m"
# "rl_outstream = stderr;" поместить в мейн.
# Compile and set executable rights
make -C ../ > /dev/null
cp ../minishell .
chmod 755 minishell


GOOD=0
TOTAL=0

pipe=/tmp/testpipe
trap "rm -f $pipe" EXIT
if [[ ! -p $pipe ]]; then
    mkfifo $pipe
fi

function exec_test()
{
  
  # execute commands, separated by ';' in minishell, using nfifo
  ./minishell <$pipe >msh_log 2>&-  &
  mshpid=$!
  IFS=';' read -ra CMND <<< "$@"
  for command in "${CMND[@]}"; do
    echo $command > $pipe
  done

  # only leaks mode
  if [ $LEAKSMODE  ]; then
    echo "leaks $mshpid | grep 'leaks for' | grep -v ' 0 leaks' >> leaks" > $pipe
    echo 'exit' > $pipe 
    sleep 0.02
    wait $!
    sleep 0.02
    if [[ $(cat leaks 2>&-) != "" ]]; then
      printf "\n$BOLDRED LEAKS! $YELLOW%s$RESET\n" "$@"
      rm -f leaks
    else
      printf "$BOLDGREEN%s$RESET" "✓ "
    fi
    return
  fi

  echo 'exit' > $pipe 
  sleep 0.01
  wait $!
  ES_1=$?
  TEST1=$(cat msh_log)

  # execute commands in bash
  bash < $pipe >msh_log 2>&-  &
  IFS=';' read -ra CMND <<< "$@"
  for command in "${CMND[@]}"; do
    echo $command > $pipe
  done
  echo 'exit' > $pipe
  wait $!
  ES_2=$?
  TEST2=$(cat msh_log)

  # compare result
  if [ "$TEST1" == "$TEST2" ] && [ "$ES_1" == "$ES_2" ]; then
    printf "$BOLDGREEN%s$RESET" "+ "
    ((GOOD++))
  else
    printf "\n$BOLDRED%s$RESET" "- "
    printf "$CYAN \"$@\" $RESET"
  fi
  if [ "$TEST1" != "$TEST2" ]; then
    echo
    printf $BOLDRED"Your output : \n%.20s\n$BOLDRED$TEST1\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
    printf $BOLDGREEN"Expected output : \n%.20s\n$BOLDGREEN$TEST2\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
    echo
  fi
  if [ "$ES_1" != "$ES_2" ]; then
    echo
    printf $BOLDRED"Your exit status : $BOLDRED$ES_1$RESET\n"
    printf $BOLDGREEN"Expected exit status : $BOLDGREEN$ES_2$RESET\n"
    echo
  fi
  ((TOTAL++))
  sleep 0.08
}

if [ "$1" == "" ] || [ "$1" == "help" ]; then
  printf "$BOLDMAGENTA\n\tAvailable arg: \n$YELLOW\tall$RESET echo cd pipe env export redirect multi syntax exit\n\
      $GREEN\tbonus$RESET quote wildcard oper\n"
fi
if [ "$1" == "all" ]; then
  printf "$BOLDMAGENTA    _____ _ _ _____ _____ _ _ ______ _ _ \n"
  printf "| \/ |_ _| \ | |_ _|/ ____| | | | ____| | | | \n"
  printf "| \ / | | | | \| | | | | (___ | || | |  | | | | \n"
  printf "| |\/| | | | | . \` | | | \___ \|   | | | | | | \n"
  printf "| | | |_| |_| |\ |_| |_ ____) | | | | |____| |____| |____ \n"
  printf "|_| |_|_____|_| \_|_____|_____/|_| |_|______|______|______|\n$RESET"
fi

if [ "$2" == "leaks" ]; then
  printf $BOLDBLUE"\n\tONLY LEAKS MODE\n"$RESET
  LEAKSMODE=1
  rm -f leaks
fi

# MY TESTS
if [ "$1" == "my" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tUTYGETT TESTS\n"$RESET
  exec_test 'pwd'
  exec_test "ls"
  exec_test "/bin/ls"
  exec_test "ls -la"
  exec_test 'cd .. && pwd'
  exec_test 'cd .. && pwd && pwd'
  exec_test 'echo test tout'
  exec_test "true && ls"
  exec_test "/bin/ls && echo $?" 
  exec_test "/bin/ip || echo $?" 
  exec_test "/bin/aaaaaaaaaaaa || echo $?" 
  exec_test "./non_existing_file______pouette || echo $?"
  exec_test "./fail_bin/segfault || echo $?"
  exec_test "non_existing_command______pouette || echo $?"
  exec_test "./Makefile || echo $?"
  exec_test "/dev || echo $?"
  exec_test "../../../ls || echo $?"
  exec_test "'' || echo $?"
  exec_test '" 1" || echo $?'
  exec_test "' ' || echo $?"
  exec_test "/bin/ip"
  exec_test "/bin/ipaaaaaaaaaa"
  exec_test "/bin/ls aaaaaaabbbbbbbb"
  exec_test "/bin/ls aaaaaaaaaaa"
  exec_test "/bin/ls -x Makefile"
  exec_test "/bin/ls - Makefile"
  exec_test "/bin/ls -- Makefile"
  exec_test "/bin/ls"
  exec_test "/bin/df"
  exec_test "export A=1 && echo $A && unset A && echo $A"
  exec_test "export A=1 && echo $A && export A=2 && echo $A"
  exec_test "unset PATH && ls"
fi

# ECHO TESTS
if [ "$1" == "echo" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tECHO TESTS\n"$RESET
  exec_test 'echo test tout'
  exec_test 'echo test tout'
  exec_test 'echo -n test tout'
  exec_test 'echo -n -n -n test tout'
  exec_test 'echo -n -n -n bonjour'
  exec_test 'echo "-n" bonjour'
  exec_test 'echo -n"-n" bonjour'
  exec_test 'echo "-nnnn" bonjour'
  exec_test 'echo "-n -n -n"-n bonjour'
  exec_test 'echo "-n -n" bonjour'
  exec_test 'echo ~'
  exec_test "echo '~'"
  exec_test 'echo "~"'
  exec_test 'echo ~123'
  exec_test 'echo 123'
  exec_test 'echo ~/123'
  exec_test 'echo ~/123/456'
	exec_test "echo"
	exec_test "echo echo"
	exec_test "echo rhobebou"
	exec_test "echo stop barking"
	exec_test 'echo "bonjour"'
	exec_test 'echo bonjour'
	exec_test "echo 'bonjour'"
	exec_test 'echo -n bonjour'
	exec_test "echo -nn bonjour"
	exec_test "echo -n -n -n bonjour"
	exec_test "echo -n -n -n bonjour"
	exec_test 'echo "-n" bonjour'
	exec_test 'echo -n"-n" bonjour'
	exec_test 'echo "-nnnn" bonjour'
	exec_test 'echo "-n -n -n"-n bonjour'
	exec_test 'echo "-n '-n'" bonjour'
	exec_test 'echo ~'
	exec_test 'echo "~"'
	exec_test "echo '~'"
	exec_test 'echo ~123'
	exec_test 'echo 123~'
	exec_test 'echo ~/123'
	exec_test 'echo ~/123/456'
	exec_test 'echo $USER'
	exec_test 'echo "$USER"'
	exec_test 'echo "'$USER'"'
	exec_test 'echo " '$USER' "'
	exec_test 'echo text"$USER"'
	exec_test 'echo text"'$USER'" ' $USER ''
	exec_test 'echo "text"   "$USER"    "$USER"'
	exec_test "echo '              $USER          '"
	exec_test 'echo               text "$USER"            "$USER"text'
	exec_test "echo ''''''''''$USER''''''''''"
	exec_test 'echo """"""""$USER""""""""'
	exec_test "echo $USER'$USER'text oui oui     oui  oui $USER oui      $USER ''"
	exec_test "echo $USER '' $USER $USER '' $USER '' $USER -n $USER"
	exec_test "echo ' \' ' \'"
	exec_test 'echo "$USER""$USER""$USER"'
	exec_test 'echo text"$USER"test'
	exec_test 'echo $USER " "'
	exec_test 'echo "$USER$USER$USER"'
	exec_test 'echo " $USER  "'$PWD''
	exec_test 'echo " $USER  "'$PWD''
	exec_test 'echo $USER=4'
	exec_test 'echo $USER=thallard'
	exec_test 'echo $USER'
	exec_test 'echo $?'
	exec_test 'echo $USER213'
	exec_test 'echo $USER$12USER$USER=4$USER12'
	exec_test 'echo $USER $123456789USER $USER123456789'
	exec_test 'echo $USER $9999USER $8888USER $7777USER'
	exec_test 'echo $USER $USER9999 $USER8888 $USER7777'
	exec_test 'echo $USER $USER9999 $USER8888 $USER7777 "$USER"'
	exec_test 'echo "$USER=12$USER"'
	exec_test 'echo "$9USER" "'$USER=12$SOMETHING'"'
	exec_test 'echo $PWD/file'
	exec_test 'echo "$PWD/file"'
	exec_test 'echo "text" "text$USER" ... "$USER"'
	exec_test 'echo $PWD'
fi

# CD TESTS
if [ "$1" == "cd" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tCD TESTS\n"$RESET
  exec_test 'cd .. && pwd'
  exec_test 'cd /Users && pwd'
  exec_test 'cd ../ && pwd'
  exec_test 'cd . && pwd'
  exec_test 'mkdir test_dir && cd test_dir && rm -rf ../test_dir && cd . && cd .. && pwd'
  exec_test "cd && pwd"
  exec_test "cd .. && pwd"
  exec_test "cd . && pwd"
  exec_test "cd /Users && pwd"
  exec_test "cd / && pwd"
  exec_test "cd '/' && pwd"
  exec_test "cd ////// && pwd"
  exec_test "cd ./././ && pwd"
  exec_test "cd / && pwd"
  exec_test "cd '/home' && pwd"
  exec_test 'cd "$PWD/file_tests" && pwd' 
  exec_test 'cd "doesntexist" && pwd' 
  exec_test 'cd "doesntexist" 2>/dev/null' 
  exec_test "cd ../../.. && pwd"
  exec_test "cd "wtf" 2>/dev/null && pwd"
  exec_test "cd .. && pwd "
  exec_test "cd .. && pwd"
  exec_test "cd ? && pwd"
  exec_test "cd + && pwd"
  exec_test "cd _ &&  pwd"
  exec_test "cd woof && pwd"
  exec_test "cd bark bark && pwd "
  exec_test "cd '/' && pwd"
  exec_test "cd $PWD/file_tests && pwd"
  exec_test "cd $OLDPWD/something && pwd"
fi


# ERROR TESTS
if [ "$1" == "error" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tERROR TESTS\n"$RESET
exec_test "test"
exec_test "| test"
exec_test "echo > <"
exec_test "echo | |"
exec_test "<"
exec_test "|"
exec_test "| | |"
exec_test "hello world"
exec_test "||||||||"
exec_test "            "
exec_test "cat wouaf wouaf"
exec_test ">"
exec_test ">>>>>>>>"
exec_test "<<<<<<<<<"
exec_test "> > > >"
exec_test ">> >> >> >>"
exec_test "~"
exec_test "<<"
exec_test "/Users"
exec_test " "
exec_test "/"
exec_test '\\\'
exec_test "EechoE"
exec_test ".echo."
exec_test ">echo>"
exec_test "<echo<"
exec_test ">>echo>>"
exec_test "echo"
exec_test "|echo|"
exec_test "echo -n"
exec_test "echo -n"
exec_test "echo"
exec_test "echo something"
exec_test "rm -f something"
exec_test "cat something"
exec_test "| echo -n oui"
exec_test "echo -n oui"
exec_test "trying to destroy your minishell"
exec_test "trying something again echo if you see this message thats not a good news"
exec_test "qewew"
exec_test "wtf"
exec_test "hi im thallard"
exec_test "nice to meet you if these tests are green"
exec_test "your minishell is perfect"
fi


# PIPE TESTS
if [ "$1" == "pipe" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tPIPE TESTS\n"$RESET
  exec_test '/bin/ls'
  exec_test 'cat tests/lorem.txt | grep arcu | cat -e'
  exec_test 'echo test | cat -e | cat -e | cat -e |cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e'
  exec_test 'cat /dev/random | head -c 100 | wc -c'
  exec_test 'ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls'
  exec_test 'head -c 2000000 /dev/zero > big_file'
  exec_test ' cat big_file | ls'
  exec_test ' cat big_file | echo lol'
  exec_test ' echo oui | cat -e'
  exec_test 'cho oui | echo non | echo something | grep oui'
  exec_test 'cho oui | echo non | echo something | grep non'
  exec_test 'cho oui | echo non | echo something | grep something'
  exec_test 'd .. | echo "something"'
  exec_test 'd .. | echo "something"'
  exec_test 'd / | echo "something"'
  exec_test 'd .. | pwd'
  exec_test 'fconfig | grep ":"'
  exec_test 'fconfig | grep nothing'
  exec_test 'hoami | grep $USER'
  exec_test 'hoami | grep $USER > tmp/file'
  exec_test 'hoami | cat -e | cat -e > tmp/file '
  exec_test 'at Makefile | grep "FLAGS"'
  exec_test 'at Makefile | cat -e | cat -e'
  exec_test 'at Makefile | grep "FLAGS" | grep "FLAGS" | cat -e'
  exec_test 'xport TEST=123 | cat -e | cat -e'
  exec_test 'nset TEST | cat -e'
  exec_test 'cho test | cat -e | cat -e | cat -e'
  exec_test 'hereis ls | cat -e | cat -e > test'
  exec_test 'cho test | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e'
  exec_test 's -la | grep "."'
fi


# ENV EXPANSIONS
if [ "$1" == "env" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tENV EXPANSIONS TESTS\n"$RESET
	exec_test 'echo test test'
	exec_test 'echo test'
	exec_test 'echo $TEST'
	exec_test 'echo "$TEST"'
	exec_test "echo '$TEST'"
	exec_test 'echo "$TEST$TEST$TEST"'
	exec_test 'echo "$TEST$TEST=lol$TEST"'
	exec_test 'echo " $TEST lol $TEST"'
	exec_test 'echo $TEST$TEST$TEST'
	exec_test 'echo $TEST$TEST=lol$TEST""lol'
	exec_test 'echo $TEST lol $TEST'
	exec_test 'echo test "$TEST" test "$TEST " test'
	exec_test 'echo "$=TEST"'
	exec_test 'echo "$"'
	exec_test 'echo "$?TEST"'
	exec_test 'echo $TEST $TEST'
	exec_test 'echo "$1TEST"'
	exec_test 'echo "$T1TEST"'
fi

# EXPORT
if [ "$1" == "export" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tEXPORT TESTS\n"$RESET
  ENV_SHOW="env | sort | grep -v SHLVL | grep -v _="
  EXPORT_SHOW="export | sort | grep -v SHLVL | grep -v _= | grep -v OLDPWD"
  exec_test 'export = '  
  exec_test 'export 1TEST= ' 
  exec_test 'export TEST ' 
  exec_test 'export ""=""  '
  exec_test 'export TES=T=""' 
  exec_test 'export TE+S=T=""' 
  exec_test 'export TEST=LOL && echo $TEST'
  exec_test 'export TEST=LOL && echo $TEST$TEST$TEST=lol$TEST'
  exec_test 'export TEST1=LOL TEST2=PIKAPIKA && echo $TEST1 && echo $TEST2' 
  exec_test 'export TEST1=LOL TEST2' 
  exec_test 'export TEST=LOL && unset TEST && echo $TEST' 
  exec_test 'export TEST="ls -l - a" && echo $TEST'
  exec_test 'export | grep "SHLVL"'
  exec_test 'export $? '
  exec_test 'export TEST'
  exec_test 'export TEST='
  exec_test 'export TEST=123'
  exec_test 'export ___TEST=123'
  exec_test 'export ""=""'
  exec_test 'export ''='''
  exec_test 'export "="="="'
  exec_test "export '='='='"
  exec_test 'export TE\\\ST=100'
  exec_test 'export TEST-=100'
  exec_test 'export _TEST=100'
  exec_test 'export | grep "SHLVL"'
  exec_test 'export TEST'
  exec_test 'export =========='
  exec_test 'export 1TEST='
  exec_test 'export TEST'
  exec_test 'export ""=""'
  exec_test 'export TES=T=""'
  exec_test 'export TE+S=T=""'
  exec_test 'export TES\\\\T=123'
  exec_test 'export TES.T=123'
  exec_test 'export TES\\\$T=123'
  exec_test 'export TES\\\\T'
  exec_test 'export TES.T=123'
  exec_test 'export TES+T=123'
  exec_test 'export TES=T=123'
  exec_test 'export TES}T=123'
  exec_test 'export TES{T=123'
  exec_test 'export TES-T=123'
  exec_test 'export _TEST=123'
  exec_test 'export TES_T=123'
  exec_test 'export TEST_=123'
  exec_test 'export TE*ST=123'
  exec_test 'export TES#T=123'
  exec_test 'export TES@T=123'
  exec_test 'export TES!T=123'
  exec_test 'export TES$?T=123'
  exec_test 'export =============123'
  exec_test 'export +++++++=123'
  exec_test 'export ________=123'
  exec_test 'export export'
  exec_test 'export echo'
  exec_test 'export pwd'
  exec_test 'export cd'
  exec_test 'export export'
  exec_test 'export unset'
  exec_test 'export sudo'
  exec_test 'export TES^T=123'
  exec_test 'export TES!T=123'
  exec_test 'export TES\~T=123'

fi

if [ "$1" == "unset" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tUNSET TESTS\n"$RESET
exec_test "unset . || echo $?"
exec_test "unset / || echo $?"
exec_test "unset ] || echo $?"
exec_test "unset [ || echo $?"
exec_test "unset , || echo $?"
exec_test "unset @ || echo $?"
exec_test "unset + || echo $?"
exec_test "unset % || echo $?"
exec_test "unset ^ || echo $?"
exec_test "unset ! || echo $?"
exec_test "unset ? || echo $?"
exec_test "unset cat=. || echo $?"
exec_test "unset cat=/ || echo $?"
exec_test "unset cat=] || echo $?"
exec_test "unset cat=[ || echo $?"
exec_test "unset cat=; || echo $?"
exec_test "unset cat=: || echo $?"
exec_test "unset cat=, || echo $?"
exec_test "unset cat=@ || echo $?"
exec_test "unset cat=+ || echo $?"
exec_test "unset cat=% || echo $?"
exec_test "unset cat=^ || echo $?"
exec_test "unset cat=! || echo $?"
exec_test "unset cat=? || echo $?"
exec_test "unset _cat || echo $?"
exec_test "unset %cat || echo $?"
exec_test "unset 4cat || echo $?"
exec_test "unset ca_t || echo $?"
exec_test "unset ca%t || echo $?"
exec_test "unset ca4t || echo $?"
exec_test "unset _cat=wwooff || echo $?"
exec_test "unset cat=_wwooff || echo $?"
exec_test "unset %cat=wwooff || echo $?"
exec_test "unset cat=%wwooff || echo $?"
exec_test "unset cat=wwooff || echo $?"
exec_test "unset cat=%wwooff || echo $?"
exec_test "unset 4cat=wwooff || echo $?"
exec_test "unset cat=4wwooff || echo $?"
exec_test "unset c+at=wwooff || echo $?"
exec_test "unset cat=w+wooff || echo $?"
exec_test "unset ca%t=wwooff || echo $?"
exec_test "unset cat=wwoo%ff || echo $?"
exec_test "unset ca_t=wwooff || echo $?"
exec_test "unset cat=wwo_off || echo $?"
exec_test "unset ca4t=wwooff || echo $?"
exec_test "unset cat=wwo4off || echo $?"
fi

# REDIRECTIONS
if [ "$1" == "redirect" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tREDIRECTION TESTS\n"$RESET
  exec_test 'echo test > ls && cat ls'
  exec_test 'echo test > ls && echo test >> ls && echo test>> ls && echo test >> ls && cat ls'
  exec_test '> lol echo test lol && cat lol && rm -rf lol'
  exec_test 'cat < ls'
  exec_test 'cat <ls'
  exec_test 'echo 1 > 1 && cat 1'
  exec_test 'echo 1 > 1 > 2 && cat 2'
  exec_test 'echo 1 > 1 > 2 > 3 && cat 3'
  exec_test 'echo 1 > 1 > 2 > 3 && cat 3'
  exec_test 'echo 1 > 1 > 2 > 3 > 4 && cat 4'
  exec_test 'pwd > pwd && cat pwd'
  exec_test 'pwd > pwd && cat pwd | cat -e'
  exec_test 'cat < ls < ls'
fi


# MULTI TESTS
if [ "$1" == "multi" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tMULTI TESTS\n"$RESET
  exec_test 'echo "test 1  | and 2" >>lol && cat <lol && cat Makefile <lol | grep minishell && rm -rf lol'
  exec_test 'unset PATH && /bin/ls'
  exec_test 'unset PATH && ./Makefile'
  exec_test 'echo 5 > ls && < 5 cat'
  exec_test 'ls | echo 6 > ls && <6 cat'
  exec_test 'cd && unset HOME && cd'
  exec_test 'cd .. && pwd'
  exec_test 'cd .. > 1 | pwd'
  exec_test 'pwd > 1'
  exec_test 'pwd > 1 && cat 1'
  exec_test 'pwd > 1 && pwd'
  exec_test 'pwd > 1 | pwd'
  exec_test 'pwd && unset HOME && pwd'
  exec_test 'ls | export TEST=5 && echo $TEST'
  exec_test 'export TEST1=LOL TEST2=PIKAPIKA'
  exec_test 'unset TEST1 TEST2'
  exec_test 'echo $TEST1'
  exec_test 'echo $TEST2'
fi

# SYNTAX 
if [ "$1" == "syntax" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tSYNTAX\n"$RESET
  exec_test ''
  exec_test '" echo" lol'
  exec_test '"echo " lol'
  exec_test '" echo" | " pwd"'
  exec_test '"echo " | "pwd "'
  exec_test '/bin/lsa'
  exec_test './Makefile || echo $?'
  exec_test '| test'
  exec_test 'echo > <'
  exec_test 'echo | |'
  exec_test 'echo "||"'
  exec_test '<'
  exec_test "grep -z"
  exec_test "ls'| 'wc -l"
  exec_test "/ls"
fi

# EXIT
if [ "$1" == "bonus" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tEXIT\n"$RESET
  exec_test "exit 42"
  exec_test "exit 42 53 68"
  exec_test "exit 259"
  exec_test "exit 9223372036854775807"
  exec_test "exit -9223372036854775808"
  exec_test "exit -4"
  exec_test "exit wrong"
  exec_test "exit  2 wrong_command"
  exec_test "gdagadgag"
  exec_test "ls -Z"
  exec_test "cd gdhahahad"
  exec_test "ls -la | wtf || echo $?"
fi

# BONUS QUOTES
if [ "$1" == "bonus" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tBONUS QUOTE\n"$RESET
  exec_test "echo '"$USER"'"
  exec_test "echo "'$USER'""
fi

# BONUS WILDCARD
if [ "$1" == "bonus" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tBONUS WILDCARD\n"$RESET
  exec_test "echo * | wc"
  exec_test "echo * | cat -e"
  exec_test "echo *"
  exec_test "mkdir empty && cd empty && touch 1 lol help me && echo * && cd .. && rm -rf empty"
fi

# BONUS OPERATOR && || ()
if [ "$1" == "bonus" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tBONUS OPERATOR \$\$ || () \n"$RESET
  
  exec_test "true && ls"
  exec_test "false&&ls"
  exec_test "true||ls"
  exec_test "false || ls"
  exec_test "true || echo 1 && echo 2"
  exec_test "false || echo 1 && echo 2"
  exec_test "true || (echo 1 && echo 2)"
  exec_test "true || echo 1 && echo 2 || echo 3"
  exec_test "(ls)"
  exec_test "( ls )"
  exec_test "true || (echo 1 && echo 2) || echo 3"
  exec_test "true || (echo 1 && echo 2) && echo 3"
  exec_test "(true || (echo 1 && echo 2) && echo 3)"
  exec_test "true || ((echo 1 && echo 2) && echo 3)"
  exec_test "()"
  exec_test " ls )"
  exec_test "touch 1 && (ls && pwd) && rm -rf 1"
fi


if [[ "$1" != "" ]] && (( $TOTAL > 0)); then
  PROCENT=$(( $GOOD * 100  / $TOTAL ))
  if (( $PROCENT > 90)); then
    printf $GREEN
  elif (( $PROCENT <= 90 )) && (( $PROCENT > 60 )); then
    printf $YELLOW
  else  
    printf $RED
  fi
  printf "\nPASS: $GOOD / $TOTAL ($PROCENT%%)$RESET\n"
fi

rm -f $pipe lol ls 1 test big_file msh_log leaks pwd
