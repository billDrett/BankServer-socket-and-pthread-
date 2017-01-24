
<h1/>BankServer(sockets, pthreads) </h1>
<h2/>Introduction </h2>
A network application with sockets and threads (Pthreads). The bank server keeps data for bank accounts and bank clients are connecting to the server to make transactions. The server has a thread pool and uses a thread for each client connection request. To keep the data for each account is uses a threadsafe hashtable. I have implement my own list, queue and hashtable because i wasnt allowed to use stl in my class.
<h2/>Compile</h2>
Make command to compile and make clean to remove all object files.
<h2/>Run	</h2></h2>
<b/>For the server</b></br>
./server -p &lt; port &gt; -s &lt;thread_pool_size&gt; -q &lt;queue_size&gt;</br>

&lt;port&gt;: port number which will be used by the socket to receive connection from clients</br>
&lt;thread_pool_size&gt;: The number of working threads in the thread pool</br>
&lt;queue_size&gt;: The size of the queue</br>

<b/>For the client</b></br>
./client -h &lt;server_host&gt; -p &lt;server_port&gt; -i &lt;command_file&gt; </br>
&lt;server_host&gt;: The address (name) of the server machine</br>
&lt;server_port&gt; : The port of the server, so the client can connect to it</br>
&lt;command_file&gt; : The file with the commands to the server</br>

<h5/>Commands</h5>
<lu>
<li>add_account &lt;init_ammount&gt; &lt;name&gt; [delay]</li>
Creates and account
<li>add_transfer &lt;amount&gt; &lt;src_name&gt; &lt;dst_name&gt;</li>
Transfers the amount from the src_name to dst_name account
<li>add_multi_transfer &lt;amount&gt; &lt;src_name> &lt;dst_name1&gt; &lt;dst_name2&gt; ... [delay]</li>
Transfers to multiple accounts
<li>print_balance &lt;name&gt;</li>
Prints the balance of the account
<li>print_multi_balance &lt;name1&gt; &lt;name2&gt; ...</li>
Prints the multiple balances of the accounts
<li>sleep &lt;time&gt;</li>
The client sleeps for time seconds
<li>exit</li>
The client closes

<h2/>Functionality</h2>
A user can register and login to the forum(if its a visitor, he skips that step), after that he/she can browse the forums and the subforms throught the menu. If its a admin he can create forums. The user can create threads and add his own posts to the threads already created from other users. Admins and moderators can change already existing posts and threads. Admins can also make a user moderator or admin.

<h2/>To-do</h2>
<lu>
<li>Add sockets for network communication</li>
<li>Add threads(pthreads) to simulate a server and support of multiple users at the same time</li>
<li>Translate the menu from greek to english</li>
</lu>
