.. 2011/07/23 uechoco 9de84d1fcc3fb0f641efa5b36973ab95cddf5faa
.. index::
   single: Installation

Symfony のインストールと設定
==================================

この章では、Symfony をベースにした動くアプリケーションが使える状態にすることを目標にしています。
幸運なことに、Symfony では「ディストリビューション」を提供しています。
これは Symfony の実用的な「スターター」プロジェクトで、ダウンロードしたらすぐに開発が始められます。

Symfony2 ディストリビューションのダウンロード
----------------------------------------------

.. tip::

    まずはじめに、PHP 5.3.2 以上が動作するウェブサーバ(Apache など)がインストールされて設定済みかどうか確認して下さい。
    Symfony2 の必要条件について詳しくは、\ :doc:`Symfony2 の実行に必要な要件</reference/requirements>` を参照してください。

Symfony2 は「ディストリビューション」をパッケージングしています。
これは Symfony2 コアライブラリ、厳選された便利なバンドル、気の利いたディレクトリ構造、
そしていくつかの初期設定を含んだ、完全に実用的なアプリケーションです。
Symfony2 のディストリビューションをダウンロードすることは、
アプリケーションをすぐに開発し始められる実用的なアプリケーションのスケルトンをダウンロードしている事にもなります。

はじめに Symfony2 ダウンロードページ(\ `http://symfony.com/download`_\ )にアクセスしてください。
ページ上に *Symfony Standard Edition* という文字が見えると思います。
これは Symfony2 のメインのディストリビューションです。
ここでは２つの項目について選ぶ必要があります。

* ``.tgz`` 形式か \ ``.zip`` 形式のアーカイブをダウンロードします - どちらも中身は同じですので、
  使いやすい方を選んでください。

* ディストリビューションにベンダーを含めたものか含めないものをダウンロードします。
  `Git`_ がインストール済みであれば 「without vendors (ベンダーライブラリを含めない物)」をダウンロードするべきです。
  そうすることで、サードパーティーやベンダーのライブラリを含めるときに、多少柔軟に追加ができるでしょう。

アーカイブの中の１つをローカルウェブサーバーの root ディレクトリの何処かにダウンロードして解凍してください。
UNIX のコマンドラインから、以下のコマンドのどれかを実行すると解凍できます(\ ``###`` を実際のファイル名に置換してください\ )。

.. code-block:: bash

    # .tgz ファイル用
    tar zxvf Symfony_Standard_Vendors_2.0.###.tgz

    # .zip ファイル用
    unzip Symfony_Standard_Vendors_2.0.###.zip

解凍し終わったら、以下のような ``Symfony/`` ディレクトリがあるでしょう。

.. code-block:: text

    www/ <- ウェブルートディレクトリ
        Symfony/ <- 解凍したディレクトリ
            app/
                cache/
                config/
                logs/
            src/
                ...
            vendor/
                ...
            web/
                app.php
                ...

ベンダーの更新
~~~~~~~~~~~~~~~~

最後に、もし「without vendors」のアーカイブをダウンロードしたならば、
以下のコマンドをコマンドラインから実行してベンダーをインストールして下さい。

.. code-block:: bash

    php bin/vendors install

このコマンドは Symfony自体を含む必要なベンダーライブラリすべてを ``vendor/`` ディレクトリにダウンロードします。

設定とセットアップ
~~~~~~~~~~~~~~~~~~~~~~~

この時点で、必要とされるサードパーティのライブラリすべてが、 ``vendor/`` ディレクトリに存在します。
また標準のアプリケーションが ``app/`` ディレクトリにセットアップされ、
いくつかのサンプルコードが ``src/`` ディレクトリの中にあります。

Symfony2 には、ウェブサーバーと PHP が Symfony を使うために設定されているかどうかを確認する手助けとなるように、
仮想のサーバー設定テスターが同梱されています。以下の URL を使って設定を確認してみてください。

.. code-block:: text

    http://localhost/Symfony/web/config.php

何か問題がある場合は、先に進む前に今のうちに修正してください。

.. sidebar:: パーミッション設定

    よくある問題としては、 ``app/cache`` と ``app/logs`` ディレクトリが、ウェブサーバーとコマンドラインの
    どちらのユーザーでも書き込み可能でなければならないことです。
    UNIX システム上でウェブサーバーのユーザーとコマンドラインユーザーが異なる場合は、
    以下のコマンドをプロジェクト内で1度実行するだけで、パーミッションを適切にセットアップされるでしょう。
    ``www-data`` はウェブサーバーのユーザーに、\ ``yourname`` はコマンドラインユーザーに置き換えてください。

    **1. chmod +a コマンドをサポートしているシステム上で ACL を使う**

    .. code-block:: bash

        多くのシステムでは ``command +a`` コマンドが使えます。
        まず最初にこのコマンドを試してみてください。
        もしエラーが起きた場合は、次の方法を試してみてください。

        rm -rf app/cache/*
        rm -rf app/logs/*

        sudo chmod +a "www-data allow delete,write,append,file_inherit,directory_inherit" app/cache app/logs
        sudo chmod +a "yourname allow delete,write,append,file_inherit,directory_inherit" app/cache app/logs

    **2. chmod +a コマンドをサポートしていないシステム上で ACL を使う**

    Ubuntu などの幾つかのシステムでは ``chmod +a`` コマンドをサポートしていません。
    ですが ``setfacl`` と呼ばれる別のユーティリティをサポートしています。
    いくつかのシステムでは、以下のコマンドを使う前にインストールする必要があるかもしれません。

    .. code-block:: bash

        sudo setfacl -R -m u:www-data:rwx -m u:yourname:rwx app/cache app/logs
        sudo setfacl -dR -m u:www-data:rwx -m u:yourname:rwx app/cache app/logs

    **3. ACL を使わない方法**

    もしディレクトリの ACL を変更する方法がなければ、
    cache と log ディレクトリにグループ書き込み権限かワールド書き込み権限
    (ウェブサーバーのユーザーとコマンドラインユーザーが同じグループかどうかに依存する)を与えるために
    umask を変更する必要があります。

    これを成功させるためには、以下の行を ``app/console``\ 、\ ``web/app.php``\ 、\ ``web/app_dev.php`` の
    ファイルの先頭に記述します。

    .. code-block:: php

        umask(0002); // This will let the permissions be 0775

        // or

        umask(0000); // This will let the permissions be 0777

    umask の変更はスレッドセーフではないため、これらのファイルにアクセスする場合は
    ACLを使うことをおすすめしています。

すべてが上手くいったら、最初の「リアルな」\ Symfony2 のウェブページをリクエストするために
「Go to the Welcome page(ウェルカムページに行く)」をクリックしましょう。

.. code-block:: text

    http://localhost/Symfony/web/app_dev.php/

Symfony2 今までの一苦労を労ってくれるでしょう。

.. image:: /images/quick_tour/welcome.jpg

開発を始める
------------

今や完全に実用的な Symfony2 アプリケーションを持っているわけなので、
開発を始められます！ディストリビューションの中にはいくつかのサンプルコードが含まれているでしょう。
その中に含まれる ``README.rst`` ファイルを(テキストファイルとして開いて)確認し、
どんなサンプルコードが含まれていてどのように後で消せるのかを学んでください。

もし Symfony が初めてでしたら、\ ":doc:`page_creation`" を御覧ください。
ページの作り方、設定の変え方、など新しいアプリケーションに必要なすべきことが載っています。

ソースコード管理システムで管理するには
--------------------------------------

``Git`` や ``Subversion`` のようなバージョンコントロールシステムを使っている場合は
バージョンコントールシステムのセットアップやいつも通りにプロジェクトをコミットし始めることができます。
``Git`` であれば次のコマンドで簡単にセットアップできます。

.. code-block:: bash

    git init

Git のセットアップや使い方について詳しい情報は、
`GitHub Bootcamp`_ のチュートリアルを調べてみてください。

``vendor/`` ディレクトリを除外する
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*without vendors* アーカイブをダウンロードしていれば
``vendors/`` ディレクトリ全体が安全に除外してソースコントロールにコミットされないようにできます。
``Git`` を使えば、次のような ``.gitignore`` ファイルを作成して追加するだけで完了します。

.. code-block:: text

    vendor/

これで vendor ディレクトリはソースコントロールにコミットされなくなったでしょう。
他の誰かがプロジェクトをクローンしたりチェックアウトする時に、
その人は必要なベンダーライブラリ全てをダウンロードするために
``php bin/vendors install`` というスクリプトを実行するだけで良いので、
とても良いと思います(本当に素晴らしいと思います!)。


.. _`http://symfony.com/download`: http://symfony.com/download
.. _`Git`: http://git-scm.com/
.. _`GitHub Bootcamp`: http://help.github.com/set-up-git-redirect
