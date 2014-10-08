---
layout: post
title:  "How You Can Get Involved"
date:   2014-10-05 22:34:25
categories: Help Git

image: /JceManager/assets/images/posts/big-data.jpg
img: big-data.jpg
---

# <i class="fa fa-hand-o-up"></i> Okay, You want to get involved...
######*There are a few ways you can help*

First of all, thank you for using our software, And even more thanks for your willing to help! Lest start from the top. 

No, you don’t need to know how to code in order to help us out! Yes, I’ll say it again in a different way…Help is not always contributing by code.  There are several ways of doing so:

 - [Asking for new features][ask]
 - [Posting bug reports][bugs]
 - [Code][code]

In this post we will cover all methods in great detail.<br>
Note that all these methods are welcome in all Open-Source software and
it will help you getting involved in most of them.

> You can click on one of the above to skip to that section.

#<a name="ask"></a><i class="fa fa-star"></i> Asking For New Features
######*You love the software and you have a great idea*

If that is the case, and you do have a great idea. The simple way of doing so is to contact the Developer. <br>You can send [Liran][liran] Or [Sagi][sagi] an e-mail.
At the mail subject please write **"JCE Manager - feature request"**

>Please do not send twice, and do not get angry if we don't reply right away. We both are students (Just like you) and we code on our free time.

The other way, is to head over to our [<i class="fa fa-github"></i> github issues page][githubissue].
Now just create a new issue with the title <br>**"feature request - (a short description)"**
<br>
At the body please write as much as you can, so we would understand and get to work.

>**Note:** We might not accept all features, some might not be as important as the others and we simply need to manage our time. 

[<i class="fa fa-arrow-up"></i> Back To The Top][top]

---

#<a name="bugs"></a><i class="fa fa-bug"></i> Posting Bug Reports
######*JCE Manager Crashed? Something wen wrong?*

Okay, sorry if something went wrong... we really test the software like **CrAzY** before we upload a new release. But if you are here, then something unacceptable happend.

For bugs, please do not send us e-mail.  It will get lost in our inbox.

Go to our [<i class="fa fa-github"></i> github issues page][githubissue] 
and browse through the issues. try to see if someone has the same issue. If so, Click on that issue and post your experience. Also, paste the content of your log file.

If your issue is new, you are more than welcome to open an issue post. Make sure that the title is short and informative.
In the body, try to provide steps to reproduce + The problem + log file. <br>For example:

```
1. login
2. click on GPA tab
3. click on clear

Problem: Software crash

Log:

17/09/2014 00:07:13	DEBUG 		 Start : JCE Manager Launched int main(int, char**)
17/09/2014 00:07:13	DEBUG 		 Local : Hebrew Local Loaded
17/09/2014 00:07:36	DEBUG 		 CSV : Getting path for csv file from user...
17/09/2014 00:07:42	DEBUG 		 CSV : User Chose:  "/home/username/test.csv"
17/09/2014 00:07:42	DEBUG 		 CSV : Atempting to export the Schedule...
17/09/2014 00:07:42	DEBUG 		 CSV : Exported Successfully
17/09/2014 00:07:47	DEBUG 		 End : JCE Manager Ended Successfully With A Return Code:  0

```

>**Note:** The Log file will be in the application folder. the name of the file is JMLog.log


[<i class="fa fa-arrow-up"></i> Back To The Top][top]

---

#<a name="code"></a><i class="fa fa-code"></i> Code
######*"Talks are cheap, Show me the code"*

If you got this far, I can asume that you know some basic stuff.

Please make sure that you have the following:

1. [Qt Creator][qt]
2. [git][git]
3. [a github account][github]

>**Note:** If you want to program under Windows, you might come to some issues.
Just Google it. 
UNIX and UNIX like systems are most recomended (Linux / Mac OSX)

**Install [qt creator][qt]**. Download the installer... (I don't need to explain how right?).

**Install <i class="fa fa-code-fork"></i>[git][git]:**<br>
Under linux just use your package manager, apt-get, yum , pacman...<br>
For example:
{% highlight bash %}
#Under Ubuntu
$ sudo apt-get install git-core
#under Arch linux
$ sudo pacman -S git
{% endhighlight %}

Under Mac OSX:
If you have Homebrew installed, install Git via the command
{% highlight bash %}
$ brew install git
{% endhighlight %}
If not, download the installer...

Under Windows: Download the Git installer from the website, at the Installer Wizard, make sure you use git Bash.  Now cross your fingers and hope that it goes well…


**Create A <i class="fa fa-github"></i> Github account**<br>
Go to [github.com][github] and sign up.


Okay, Now we have everything we need to get started. well, one more thing...<br>
If it is the first time you installed <i class="fa fa-code-fork"></i>[git][git] we need to set it up.

>**Note:** We are not going to learn about  <i class="fa fa-code-fork"></i> git, If you dont know
how to use it, Google it! It's essential for collaborating with us, and in general it's really good to know.

To Setup <i class="fa fa-code-fork"></i>git, fire up a terminal (in Windows git bash... or something like it... please choose a better OS <i class="fa fa-smile-o"></i>).

And type:
{% highlight bash %}
# Setup your name 
$ git config --global user.name "YOUR NAME"
# Setup your email - same as e-mail at github!
$ git config --global user.email "YOUR EMAIL ADDRESS"
# For having some Color indicators in the terminal
$ git config --global color.ui auto
{% endhighlight %}

**And We Are Ready!!**

To start messing arround with the code, you will want to fork the repo.
Go To [*JCE Manager* github repo][repo], and click on the "Fork" button on top.
that will create a copy of the software on your account.

Then You will want to pull the source code to your computer.
copy the URL address of the copy you've just created (should be some thing like: https://github.com/yourusername/JceManager.git)

Let's call that URL **repoPath** from now on.

Now navigate to your working directory, in my case Dev folder.
You will then f

{% highlight bash %}
~$ cd Dev
~/Dev/$ git clone repoPath
# Now You Have The Code on your local computer...
~/Dev/$ cd JceManager
~/Dev/JceManager$ ls | grep *.pro
#the file you see at the output is the qt creator project file... you are ready to code...
{% endhighlight %}


At this point you can do whatever you want with the code and push to your github account.
If you want us to pull code from you, it could be a bug fix, a new feature you coded or whatnot just send us a pull request at github!




[<i class="fa fa-arrow-up"></i> Back To The Top][top]

---

[ask]: #ask
[bugs]: #bugs
[code]: #code
[liran]: mailto:liranbg@gmail.com
[sagi]: mailto:sagidayan@gmail.com
[githubissue]: https://github.com/liranbg/JceManager/issues
[top]: #topofpage
[qt]: http://qt-project.org/
[git]: http://git-scm.com/
[github]: https://github.com/
[repo]: https://github.com/liranbg/JceManager