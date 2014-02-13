#here is what is in my .profile
#following are for lab3###########################################

#do module load all if on dplsubmit
[ `hostname` = dplsubmit ] && module load all


#display status of defined friends
friends=( "droessmj" "savidespk" )

w > users.log
for user in ${friends[@]};
	do 
	   if echo users.log | grep -q $user
		then echo "$user is online"
		else lastlog -u $user
	   fi
done
rm users.log

#display a fortune
fortune


#below is what happens when source .profile

Script started on Thu 13 Feb 2014 02:25:37 PM CST
14:25 droessmj in  ~ $ source .profile 
Username         Port     From             Latest
droessmj                                   **Never logged in**
lastlog: Unknown user or range: savidespk
The bone-chilling scream split the warm summer night in two, the first
half being before the scream when it was fairly balmy and calm and
pleasant, the second half still balmy and quite pleasant for those who
hadn't heard the scream at all, but not calm or balmy or even very nice
for those who did hear the scream, discounting the little period of time
during the actual scream itself when your ears might have been hearing it
but your brain wasn't reacting yet to let you know.
		-- Winning sentence, 1986 Bulwer-Lytton bad fiction contest.
14:25 droessmj in  ~ $ exit
exit

Script done on Thu 13 Feb 2014 02:25:47 PM CST
