パッチの投稿
============

Symfony2 へ機能改善などを提案する最も効果的な方法は、パッチを投稿することです。

チェックリスト
--------------

パッチを投稿する際に次のチェックリストに従うことで、レビュー時の不要なフィードバックループを避け、より迅速に Symfony2 本体へ取り込めるようになります。

プルリクエストの説明には、必ず次のテンプレートを使ってください。

.. code-block:: text

    Bug fix: [yes|no]
    Feature addition: [yes|no]
    Backwards compatibility break: [yes|no]
    Symfony2 tests pass: [yes|no]
    Fixes the following tickets: [comma separated list of tickets fixed by the PR]

記述例は次のとおりです:

.. code-block:: text

    Bug fix: no
    Feature addition: yes
    Backwards compatibility break: no
    Symfony2 tests pass: yes
    Fixes the following tickets: -

パッチ投稿時にこのテンプレートに従うよう、協力をお願いします。

.. tip::

    機能追加のリクエストについては、必ず "master" ブランチへ送信してください。バグフィックスの場合は、アクティブなブランチのうち最も古いものへ送信してください。
    また、パッチで後方互換性が損なわれてはいけません。
    パッチで、たとえばパスしないテストが残っている等、完了していない作業がある場合は、パッチのタイトルに "[WIP]" と付けてください。

初期セットアップ
----------------

Symfony2 に関する作業を始める前に、次のソフトウェアをセットアップしてください。

* Git

* PHP バージョン 5.3.2 以降

* PHPUnit 3.5.11 以降

名前とメールアドレスを次のように設定します。

.. code-block:: bash

    $ git config --global user.name "Your Name"
    $ git config --global user.email you@example.com

.. tip::

    Git に慣れていない場合は、素晴らしい、しかも無料の \ `ProGit`_\ を一読されることをおすすめします。

次の手順で Symfony2 のソースコードを取得します。

* `GitHub`_\ にアカウント登録し、サインインしてください。

* `Symfony2 リポジトリ`_\ をフォークしてください（"フォーク" ボタンをクリックします）

* "鋭意フォーク中" の処理が完了したら、フォークしたリポジトリをローカルにcloneします（次のコマンドで、\ `symfony`\ ディレクトリが作られます）。

.. code-block:: bash

      $ git clone git@github.com:USERNAME/symfony.git

* 上位リポジトリを\ ``remote``\ として追加します。

.. code-block:: bash

      $ cd symfony
      $ git remote add upstream git://github.com/symfony/symfony.git

これで Symfony2 リポジトリを取得できたので、\ :doc:`テストのドキュメント <tests>`\ で説明している手順にしたがって、ご自分の環境ですべてのユニットテストがパスするかどうか確認してください。

パッチを作成する
----------------

バグや機能改善のパッチを作成する場合、必ずトピックブランチを作成してください。

このブランチは、新しい機能を追加する場合は `master` ブランチを元に作成してください。
バグフィックスの場合は、Symfony のメンテナンスされているバージョンで、バグが発生した最も古いバージョン（たとえば `2.0`\ ）を元に作成してください。

トピックブランチを作成するには、次のコマンドを実行します:

.. code-block:: bash

    $ git checkout -b BRANCH_NAME master

.. tip::

    ブランチには説明的な名前を使ってください（チケット番号 `XXX` のバグ修正には、\ `ticket_XXX` というブランチ名を使うことが推奨されています）。

上のコマンドを実行すると、ワーキングツリーのコードが新しく作られたブランチに自動的に切り替わります（\ `git branch` コマンドで現在のブランチを確認できます）。

このブランチで、好きなだけ作業し、コミットできますが、次のルールに従ってください。

* :doc:`コーディング規約 <standards>`\ にしたがってください（\ `git diff --check` コマンドで行の末尾のスペースなどをチェックしてください）。

* バグが修正されたことや、新しい機能が実際に動作していることを確認するユニットテストを追加してください。

* 各コミットをアトミックで論理的に独立させてください（\ `git rebase` を使って、コミット履歴がクリーンで論理的になるようにしてください）。

* 適切なコミットメッセージを記述してください。

.. tip::

    コミットメッセージには、1行目にコミットの要約を記述し、次の行は任意で空行を入れ、さらに詳細な説明を続けます。
    要約には、コミットで作業対象となっているコンポーネント名を角括弧で囲んで記述してください（\ `[DependencyInjection]`\ 、\ `[FrameworkBundle]`\ 、...）
    その後の文は動詞で始め（\ `fixed ...`\ 、\ `added ...`\ 、...）、末尾にピリオドを付けないでください。

パッチを投稿する
----------------

パッチを投稿する前に、次の手順で自分のブランチを更新します（更新を適用するのにしばらく時間がかかる場合があります）。

.. code-block:: bash

    $ git checkout master
    $ git fetch upstream
    $ git merge upstream/master
    $ git checkout BRANCH_NAME
    $ git rebase master

`rebase` コマンドを実行した後、マージの競合を解決しなければならない場合があります。\ `git
status` コマンドを実行すると、\ *マージされなかった*\ ファイルを確認できます。すべての競合を解決し終わったら、 rebase を続行します。

.. code-block:: bash

    $ git add ... # 解決したファイルをインデックスに追加します
    $ git rebase --continue

すべてのテストがパスすることを確認してから、リモートにブランチをプッシュしてください。

.. code-block:: bash

    $ git push origin BRANCH_NAME

パッチについて `dev メーリングリスト`_\ に投稿するか、プルリクエストを送信してください（プルリクエストは、\ ``symfony/symfony``\ リポジトリへ送信してください）。
コアチームの作業が簡単になるように、以下のようにプルリクエストのメッセージに変更したコンポーネントを常に含むようにしてください。

.. code-block:: text

    [Yaml] foo bar
    [Form] [Validator] [FrameworkBundle] foo bar

メーリングリストへメールを送信する場合は、参照できるブランチの URL (\ ``http://github.com/USERNAME/symfony.git
BRANCH_NAME``\ ) またはプルリクエストの URL を記載してください。

メーリングリストや GitHub のプルリクエストへのフィードバックにしたがって、パッチを修正してください。パッチを再投稿する前に、master に対してマージではなく、必ず rebase
してください。その後、次のようにして origin に対して強制的に push します。

.. code-block:: bash

    $ git rebase -f upstream/master
    $ git push -f origin BRANCH_NAME

.. note::

    投稿したすべてのパッチは、コードの中に明白に示されていない限り、
    MIT ライセンスの元でリリースされます。

メンテナンスブランチに対するバグフィックスは、対象となるより新しいすべてのブランチへもマージされます。
たとえば、パッチを `2.0` ブランチへ投稿した場合、コアチームにより `master` ブランチへも適用されます。

.. _ProGit:              http://progit.org/
.. _GitHub:              https://github.com/signup/free
.. _Symfony2 リポジトリ: https://github.com/symfony/symfony
.. _dev メーリングリスト:    http://groups.google.com/group/symfony-devs

.. 2011/05/18 doublemarket 1697e640
.. 2011/10/05 hidenorigoto 2a6c0596172b23192b0b810b4096093aee14e334

