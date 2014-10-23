---
layout: post
title:  "How to get involve"
date:   2014-10-05 22:34:25
categories: Help Git

image: /JceManager/assets/images/posts/big-data.jpg
img: big-data.jpg
---

# <i class="fa fa-hand-o-up"></i> Getting involved
######There are few ways you can to it

First of all, we would like to thank you for using our software, And even more thanks for your willing to help!

You do not need to know how to program in order to help us out.
Help is not always contributing by code but also helping us with ideas.
There are several ways to do so:

 - [Asking for new features][ask]
 - [Posting bug reports][bugs]
 - [Develope][code]

In this post we will cover all methods in great details.<br>
Note that all these methods are welcome in all Open-Source software and
it will help you getting involved in most of them.

> You can click on one of the links above to skip to that section.

#<a name="ask"></a><i class="fa fa-star"></i> Asking for new features
######You love the software and you have a great idea

If that is the case, and you do have a great idea, The simple way of doing so is to contact the Developer.
<br>
You can send [Liran][liran] Or [Sagi][sagi] an e-mail, containing a subject "Jce Manager feature request".

The other way, is to head over to our [<i class="fa fa-github"></i> github issues page][githubissue].
Just create a new issue with the title "Feature request" and in the body give us a brief about your idea.
<br>
At the body please write as much as you can, so we would understand and get to work.

>Note: We might not accept all features, some might not be as important as the others and we simply need to manage our time. 

[<i class="fa fa-arrow-up"></i> Back To The Top][top]

---

#<a name="bugs"></a><i class="fa fa-bug"></i> Posting bug reports
######*JCE Manager Crashed? Something wen wrong?*

Something went wrong... we test the software before we upload a new release. 
But if you are here, then something went wrong... :-(

For bugs, please do not send us e-mail.  It will get lost in our inbox.

Go to our [<i class="fa fa-github"></i> github issues page][githubissue] and browse through the issues. 
try to see if someone has the same issue, If so, Click on that issue and post your experience.
Please add your JML.log (log file. it does NOT contain your password).

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

>Note: The Log file will be in the application folder. the name of the file is JMLog.log


[<i class="fa fa-arrow-up"></i> Back To The Top][top]

---

#<a name="code"></a><i class="fa fa-code"></i> Code
######*"Talks are cheap, Show me the code"*

If you got this far, I assume you know some basic stuff.

Please make sure that you have the following:

1. [Qt Creator][qt]
2. [git][git]
3. [a github account][github]

Install [qt creator][qt]

Install <i class="fa fa-code-fork"></i>[git][git]:
<br>
Under linux just use your package manager, apt-get, yum , pacman...<br>
For example:
{% highlight bash %}
#Under Ubuntu
$ sudo apt-get install git-core
#under Arch linux
$ sudo pacman -S git
{% endhighlight %}

Under Mac OSX:
<br>
If you have Homebrew installed, install Git via the command
{% highlight bash %}
$ brew install git
{% endhighlight %}
If not, download the installer...

Under Windows:
<br>
Download the Git installer from the website, at the Installer Wizard, make sure you use git Bash.


Create a <i class="fa fa-github"></i> Github account**<br>
Go to [github.com][github] and sign up.


Okay, Now we have everything we need to get started.
<br>
If it is the first time you installed <i class="fa fa-code-fork"></i>[git][git] we need to set it up.

>Note: We are not going to learn about  <i class="fa fa-code-fork"></i> git, If you dont know
how to use it, Google it! It's an essential for development collaborating.

To Setup <i class="fa fa-code-fork"></i>git, fire up a terminal (in Windows use git bash)

And type:
{% highlight bash %}
# Setup your name 
$ git config --global user.name "YOUR NAME"
# Setup your email - same as e-mail at github!
$ git config --global user.email "YOUR EMAIL ADDRESS"
# For having some Color indicators in the terminal
$ git config --global color.ui auto
{% endhighlight %}

And We Are Ready!!

To start messing arround with the code, you will want to fork the repo.
Go to [*JCE Manager* github repo][repo], and click the "Fork" button on top.
It will create a copy of the software's code on your account.

Then You will want to pull the source code to your computer.
copy the URL address of the copy you've just created (should be some thing like: https://github.com/yourusername/JceManager.git)

Let's call that URL repoPath from now on.

Now navigate to your working directory, in my case Dev folder.

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