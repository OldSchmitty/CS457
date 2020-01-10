ReadMe for homework 2, by Mark Smith on 12-09-2018

This project consists of 2 main code bases, Server and Client, which are in their respective directories.
The server accepts incomming connections and facilitates the chat rooms. The client then connects and sends through commands.
The GUI most likely seems really weak right now due to my poor knowledge on how to program for GUI's. I spent a substantial amount
of time trying to get dynamic creation and destruction to work. I still don't know if I am doing it right, but I can't find
any other ways to do it.

To compile:

Go into the desired directory and use the command -
	g++-8 *.cpp *.h -lpthread -o <outputname>

To run: ./<outputname>

Server: arguments are 
	-db <path-to-db-files>
	-port <port-number>
	-configuration <path-to-conf-file>

Files needed for -db are: 
	banner.txt		-message to display when people connect to the server.
	banusers.txt		-list of users who are currently banned
	channels		-contains channel information
	help.txt		-the help message to display when user uses HELP
	rules.txt		-Server rules for clients
	users			-contains user information

Client: arguments are
	-p <port-num>
	-h <host-name> 
	-u <user-name>
	-c <test-file>
	-l <log-file>

Supported commands for Server:
	help		shows information about commands
	userlist	shows registered users
	connections	shows how many users are currently connected

Supported Commands for Client:
	/away [<message>]		Sets status to away, will not recieve private messages and reply with <message>
					if it was given in argument.
	
	/die				Shuts the server down. Only usable by administrators.
	
	/help				Shows additional information to help use the client

	/Info				Gets relevant information on the Server including version

	/invite <channel> <user>	Notifies <user> they have been invited to they have been invited to join <channel
					If the channel is in invite only mode, it will allow <user> to join.
	
	/ison [..<user>]  		For each <user> supplied it will tell you if that user is currently connected to
					the server.

	/join <channel>			Joins a channel for chatting. Currently a user can only be in 1 channel at a time
					due to lack of gui support for multiple channels. This will be added in the future.

	/kick <channel> <user>	<message>	Kicks <user> out of <channel> and send them <message> if supplied.
						only usable by channelops or higher.

	/kill <user>			Disconnects <user> from the server. Useable only by sysops and administrators.

	/knock <channel>		If not a currentl member of <channel>, and <channel> is in invite only mode, 
					it will display a request to the channel that you would like to be invited.

	/list				Updates GUI list of channels avaiable.

	/nick <nickname>		Sets the currents clients nickname if not already registered or in use.
					If used in the combinations of /pass, /nick, and /user, will log into 
					a registered account for that nickname.

	/notice <user> <message>	Similair to /privmsg but ignores away status. Usable only by the server or levels above
					normal user.

	/part <channel>			If the client is a member of the channels the leave it.

	/pass <password>		Sets the current password used for logging in. must be used with /pass, /nick, /user.

	/ping				Requests pong reply from the server. Used for testing if you are still conected.

	/privmsg <user>|<channel <msg>	Sends message to either the supplied <user> or <channel>
	
	/quit				Exits the client and disconnects from the server.

	/rules				Displays the connected servers rules.

	/register <channelname> <pass>	Creates a user specific channel with the given password. The user will be the only chanop
					and the channel will die when the user disconnects.

	/setname <realname>		Sets current clients real-name attribute on server

	/time				Displays the servers local time.			

	/user	<user>			Sets <user> as the username to login with. Use after calling /pass and /nick to login.

	/wallops <message>		Sends <msg> to all sysops and admins. Must be sysop or admin to use.

	/version			Command is not currenlty supported but the server version can be seen with /info 


Explanation of  Server files:

Channel.cpp/h		Object that holds information about users, options, and chanops for a given channel. Has commands for 
			interacting with users.

ChatServer.cpp/h	Main driver for program. Creates a listener thread for incoming TCP connections and also creates threads 
			for communications to each client.

OutputManager.cpp	Thread for outputing messages to channels and users. IN the future this is meant to be used in conjunction
			with a thread pool for managing smooth output across the server. Uses a thread safe queue of messages to send.

Protocols.cpp/h		Event factory for parsing commands and messages and performing to proper replys and actions.

Server.cpp/h		Object that holds map of Users and map Channels.

tcpServerSocket.cpp/h	Code given from class with minor updates to allow for a listening socket to be made.

tcpUserSocket.cpp/h	Code given from class with minor changes to creat socket connections between client and user.

User.cpp/h		Object detailing user information like password, nick, if they are banned, etc.

conf directory		Contains a default conf file for the server.

db directory		Contains default files for the servers channels, users, etc.

logs			place to store log files


Explanation of Client Files:

ChatClient.cpp/h	Main driver for program. Creates a thread for listening on the tcp socket and output.

Protocols.cpp/h		Event factory for parsing commands entered by user. if no command is specified it is 
			assumed to be a PRIVMSG to the current channel.

tcpServerSocket.cpp/h	Code given from class with minor updates to allow for a listening socket to be made.

tcpUserSocket.cpp/h	Code given from class with minor changes to creat socket connections between client and user.


Notes:

Logging in-		I tried to replicate the login process as best I could from IRC. in order to login use /nick with 
			an unregistered nickname, or do a sequence of /pass, /nick, /user to log into a registered account.
			In the new GUI you can also use the login button.

Exiting-		In the Server use "EXIT" to exit the program. This will shutdown the server if it is still running.
			In the event that the server is shut down remotley, "EXIT" with still need to be run to shut down
			he program. Use "/quit" to exit client.

Channels-		Channels are automatically shown in the GUI when logging in and updated when channels are created and 
			destroyed.  	



Bug Fixes-
		tcpUserSocket:	The skelton code provided wil fail if a client sends messages rapidly, such as wehn using a
				text file. All of the messages can be revieved in the same recv() call in the socket on 
				both the servers side and the clients side. Because of the null characters in the buffer
				when sending back a string of the message, only the first message will be constructed, and
				the rest of the buffer will faile to be sent into the string. I fixed this by iterating
				through the buffer and maunally appending the extra commands into the return string. Then
				in the reciever code split the commands on the terminator ("/r/n") and process each command
				in a loop. Because most IDE's will not show the null characters in the buffer, this took me
				very long time to fix.

		Threads:	I had many issues passing containers, like maps, into a thread for access, and had to settle
				on using some pointer magic. If you try to pass any container into a thread by reference, it 
				does not have an accurate representaion of the container in the thread. I still haven't figured
				out why this is and willcontinue to investigate. Additionally, it took me a while to realize that
				in order to be thread safe in my user class (operations that can change the levels and sttus of
				the user and operations that require the status can race) that I needed to make the mutex for each
				User mutable, because the Users can move from channels and such. I previous threading experience is
				in other languages so this took me by surprise.

		cout:		If you do not flush the buffer for outputs with "<<endl" you can sometimes run into situations where
				you think ou have bugs when you don't do to multiple threads trying to output at once, and the 
				output printing in chunks.

		close socket:	Took me a while to find out how to wake up a blocking thread waiting on recieve or listen if the
				connection was closed.

		GUI Threads: 	Qt threads are the only ones that can do anything with the GUI, and there appear to be several
				race conditions that there is little documentation on how to fix. A lot of the Qt system is very
				abstracted and has poor examples when trying to do anything dynamic such as opening or closing
				new windows from multiple threads.

		



 

