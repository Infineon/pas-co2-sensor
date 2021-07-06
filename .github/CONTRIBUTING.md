Contributing Code to Infineon
============================

For many repositories, Infineon welcomes code contributions via pull requests (PRs). This document tells you what you need to do to create a successful PR and add your code to our codebase.

Infineon uses GitHub to develop and deliver code. Some repositories, as our community projects, are developed directly on GitHub, and they rely partially or entirely on the GitHub pull request workflow. 
For other software products, our code base is inside our firewall. In this case, when you create a PR, we will review, hopefully approve, and when approved pull your changes into our internal systems. Your changes will appear in the next release, with our acknowledgement for your contribution. Your PR will not be merged into the master branch. 

### Licensing

By creating this pull request (a “Contribution”), you, the submitter (“You”), agree to license your Contribution to Infineon Technologies (“Infineon”) under the following terms. As used herein Code shall mean any software that is part of the Contribution whether in source or binary form and Content shall mean anything included in the Contribution that is not Code.  Except as expressly set forth herein, Infineon does not obtain any ownership of, or other intellectual property rights, in or to Your Code or Content. You are entirely responsible for any Code and Content you submit. You hereby grant Infineon a perpetual, irrevocable, worldwide, sub-licensable, royalty-free, non-exclusive license to make, modify, have made, use, import, copy, display, and distribute Content. You further grant Infineon a license that entitles Infineon and its sublicensees to make, modify, have made, use, import, copy, display, and distribute the Code.

To the extent the Contribution is submitted by You as an individual, You hereby represent and warrant that no third party has any rights to the Contribution. Furthermore, to the extent that the Contribution is made by You as an employee, contractor for or otherwise an agent of an entity, You represent and warrant that You are doing so on behalf of that entity, You have the permission of that entity to make this Contribution and You have the authority to bind the Entity to the license to the Contribution on the terms as set forth herein.

### Coding Style

Your code must follow the practices used in the source code, which can vary depending on the nature of the code in the repo. Examine the source code for all the usual suspects: line length, naming conventions for variables and functions, if/then/else formatting, case statements, documentation in comments, and so on. Then do the same.

### Pull Request Workflow

These instructions assume you’re familiar with GitHub. If you are not, GitHub has [good help](https://help.github.com/en/github/collaborating-with-issues-and-pull-requests/proposing-changes-to-your-work-with-pull-requests) on each step in the process.

#### Talk to Us

You might want to tell us about your idea first. We do not always develop code on GitHub. Some repositories are developed using internal systems. That means that what you see in the latest commit of the master branch is *not* necessary the front edge of the code. We may already be implementing your idea! It is also possible that we are heading in a
direction that will make your idea a non-starter. A well-crafted PR is a non-zero amount of work. You can open an issue and start the discussion, and maybe save yourself some effort.

#### Fork the Repo 

Use the latest release on the default (typically master) branch. In your fork, create a branch to hold your changes, and then make changes in that branch. This keeps all your changes together and easy for us to merge into our codebase if your PR is approved.

#### Create the Pull Request

Go to the Infineon repository and create a pull request. Because your code is in your fork, on the Compare page click **compare across forks**. Then pick your fork and the branch that holds your changes. Follow the instructions that appear in the text field. For example, give your PR a good title and description, so we know what you’re proposing. “Bug fix” is not a good description.

#### Acknowledgement

The Infineon support team will acknowledge your PR, typically within two to three business days. Support notifies the engineering team that the PR is pending. Depending on schedules and workload, it may be a few days before an engineer shows up to start the review.

#### Review and Discussion

A Infineon engineer manages the review on GitHub. The following steps or events are typical:

-   we look at the code to see if you followed coding guidelines

-   we confirm that the license requirements are met

-   for internally developed repos, we pull your code into an internal test system to do a quick sanity check

-   we will go back and forth with you over the details of the code

-   the PR is either approved or rejected

For internally developed repositories, we do not merge your code into the master branch on GitHub. 

#### Approval (or not)

For community repositories directly developed in GitHub, the pull request approval or rejection is transparently stated in the GitHub pull request workflow.

In the case of internally developed repositories, when the Infineon engineering accepts a pull request, we label the PR "approved". We then relocate the code in the pull request into our internal systems. At this point what typically happens is your code is merged into our codebase and appears in the next release. Your contribution will be acknowledged in the release documentation.

Once in our internal systems the code will be fully tested in our regression test system. We may discover a problem not seen during the review. In addition, our internal codebase is advancing continuously. It is possible that a conflict will appear that was missed in the GitHub review. If something like this happens, we’ll be back on GitHub to let you know what’s going on.

Thanks for contributing!

