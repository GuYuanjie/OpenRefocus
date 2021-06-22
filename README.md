
<!-- Add banner here -->
# OpenRefocus
In order to observe targets in `expanded depths of view` in `light field imaging`, we design an open source light field `parallel refocusing` `Qt GUI software` named `OpenRefocus`,   which is based on `classical spatial projection integration`.


<!-- Add buttons here -->

<!-- Describe your project in brief -->

<!-- The project title should be self explanotory and try not to make it a mouthful. (Although exceptions exist- **awesome-readme-writing-guide-for-open-source-projects** - would have been a cool name)

Add a cover/banner image for your README. **Why?** Because it easily **grabs people's attention** and it **looks cool**(*duh!obviously!*).

The best dimensions for the banner is **1280x650px**. You could also use this for social preview of your repo.

I personally use [**Canva**](https://www.canva.com/) for creating the banner images. All the basic stuff is **free**(*you won't need the pro version in most cases*).

There are endless badges that you could use in your projects. And they do depend on the project. Some of the ones that I commonly use in every projects are given below. 

I use [**Shields IO**](https://shields.io/) for making badges. It is a simple and easy to use tool that you can use for almost all your badge cravings. -->

<!-- Some badges that you could use -->

<!-- ![GitHub release (latest by date including pre-releases)](https://img.shields.io/github/v/release/navendu-pottekkat/awesome-readme?include_prereleases)
: This badge shows the version of the current release.

![GitHub last commit](https://img.shields.io/github/last-commit/navendu-pottekkat/awesome-readme)
: I think it is self-explanatory. This gives people an idea about how the project is being maintained.

![GitHub issues](https://img.shields.io/github/issues-raw/navendu-pottekkat/awesome-readme)
: This is a dynamic badge from [**Shields IO**](https://shields.io/) that tracks issues in your project and gets updated automatically. It gives the user an idea about the issues and they can just click the badge to view the issues.

![GitHub pull requests](https://img.shields.io/github/issues-pr/navendu-pottekkat/awesome-readme)
: This is also a dynamic badge that tracks pull requests. This notifies the maintainers of the project when a new pull request comes.

![GitHub All Releases](https://img.shields.io/github/downloads/navendu-pottekkat/awesome-readme/total): If you are not like me and your project gets a lot of downloads(*I envy you*) then you should have a badge that shows the number of downloads! This lets others know how **Awesome** your project is and is worth contributing to.

![GitHub](https://img.shields.io/github/license/navendu-pottekkat/awesome-readme)
: This shows what kind of open-source license your project uses. This is good idea as it lets people know how they can use your project for themselves.

![Tweet](https://img.shields.io/twitter/url?style=flat-square&logo=twitter&url=https%3A%2F%2Fnavendu.me%2Fnsfw-filter%2Findex.html): This is not essential but it is a cool way to let others know about your project! Clicking this button automatically opens twitter and writes a tweet about your project and link to it. All the user has to do is to click tweet. Isn't that neat? -->

# Demo-Preview

<!-- Add a demo for your project -->
![OpenRefocus](https://raw.githubusercontent.com/GuYuanjie/git-imgs/main/OpenRefocus/%E7%94%A8%E6%88%B7%E4%BD%BF%E7%94%A8%E5%9B%BEV2.png)
<!-- After you have written about your project, it is a good idea to have a demo/preview(**video/gif/screenshots** are good options) of your project so that people can know what to expect in your project. You could also add the demo in the previous section with the product description.-->

# Table of contents

<!-- After you have introduced your project, it is a good idea to add a **Table of contents** or **TOC** as **cool** people say it. This would make it easier for people to navigate through your README and find exactly what they are looking for.

Here is a sample TOC(*wow! such cool!*) that is actually the TOC for this README. -->

- [OpenRefocus](#OpenRefocus)
- [Demo-Preview](#demo-preview)
- [Table of contents](#table-of-contents)
- [Installation](#installation)
- [Usage](#usage)
- [Development](#development)
- [License](#license)
- [Footer](#footer)

# Installation

<!-- *You might have noticed the **Back to top** button(if not, please notice, it's right there!). This is a good idea because it makes your README **easy to navigate.*** 

The first one should be how to install(how to generally use your project or set-up for editing in their machine).

This should give the users a concrete idea with instructions on how they can use your project repo with all the steps.

Following this steps, **they should be able to run this in their device.**

A method I use is after completing the README, I go through the instructions from scratch and check if it is working. -->

<!-- Here is a sample instruction:

To use this project, first clone the repo on your device using the command below:

```git init```

```git clone https://github.com/navendu-pottekkat/nsfw-filter.git``` -->

## If you want to use CPU mode or use GPU mode with GTX-1050Ti Download OpenRefocusRelease and open OpenRefocus.exe
>Because of the upload restriction, the program is divided into volumes in `PackingCompression.part1.rar` and `PackingCompression.part2.rar`.
## If you want to use GPU mode with more version GPUs, reconfigure CUDA setting in the `.pro` file 
```
Fermi (CUDA 3.2 and later, deprecated from CUDA 9):
SM20 or SM_20, compute_30 – Older cards such as GeForce 400, 500, 600, GT-630
Kepler (CUDA 5 and later):
SM30 or SM_30, compute_30 – Kepler architecture (generic – Tesla K40/K80, GeForce 700, GT-730)
Adds support for unified memory programming
SM35 or SM_35, compute_35 – More specific Tesla K40
Adds support for dynamic parallelism. Shows no real benefit over SM30 in my experience.
SM37 or SM_37, compute_37 – More specific Tesla K80
Adds a few more registers. Shows no real benefit over SM30 in my experience
Maxwell (CUDA 6 and later):
SM50 or SM_50, compute_50 – Tesla/Quadro M series
SM52 or SM_52, compute_52 – Quadro M6000 , GeForce 900, GTX-970, GTX-980, GTX Titan X
SM53 or SM_53, compute_53 – Tegra (Jetson) TX1 / Tegra X1
Pascal (CUDA 8 and later)
SM60 or SM_60, compute_60 – GP100/Tesla P100 – DGX-1 (Generic Pascal)
SM61 or SM_61, compute_61 – GTX 1080, GTX 1070, GTX 1060, GTX 1050, GTX 1030, Titan Xp, Tesla P40, Tesla P4
SM62 or SM_62, compute_62 – Drive-PX2, Tegra (Jetson) TX2, Denver-based GPU
Volta (CUDA 9 and later)
SM70 or SM_70, compute_70 – Tesla V100
SM71 or SM_71, compute_71 – probably not implemented
SM72 or SM_72, compute_72 – currently unknown
```

# Usage
>1. Import micro-lens coords, ours Lytro Illum demo .txt files are in `./OpenRefocusSourceCode/cameraData/`
>2. Import light field image, ours Lytro Illum demo images files are in `./OpenRefocusSourceCode/imageData/`
>3. Type parameters
>>imgHeight: refocusing image height, default `434` for ours Lytro Illum.  
>>imgWidth: refocusing image height, default `541` for ours Lytro Illum.  
>>radius: micro-lens radius, default `7` for ours Lytro Illum.  
>>depthResolution: how much refocusing images u want, often `256`.  
>>alphaMin： alpha range, default `0.2`.  
>>alphaMax:  alpha range, default `2`.  
>3. Refocusing, choose CPU or GPU mode
>4. Display, drag the slider or click on the area where u interest
>5. Save and Reboot

# Development

<!-- This is the place where you give instructions to developers on how to modify the code.

You could give **instructions in depth** of **how the code works** and how everything is put together.

You could also give specific instructions to how they can setup their development environment.

Ideally, you should keep the README simple. If you need to add more complex explanations, use a wiki. Check out [this wiki](https://github.com/navendu-pottekkat/nsfw-filter/wiki) for inspiration. -->



# License
```
MIT License

Copyright (c) 2021 Yuanjie Gu

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

# Footer
[(Back to top)](#table-of-contents)

<!-- Let's also add a footer because I love footers and also you **can** use this to convey important info.

Let's make it an image because by now you have realised that multimedia in images == cool(*please notice the subtle programming joke). -->

Leave a star in GitHub, give a clap in Medium and share this guide if you found this helpful.

<!-- Add the footer here -->

<!-- ![Footer](https://github.com/navendu-pottekkat/awesome-readme/blob/master/fooooooter.png) -->
