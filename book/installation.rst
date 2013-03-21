.. index::
   single: Installation

.. note::

    * 対象バージョン：2.2 (2.1以降)
    * 翻訳更新日：2013/3/16

Symfony のインストールと設定
============================

この章では、Symfony をインストールする方法について解説します。
Symfony では「ディストリビューション」が提供されています。
ディストリビューションとは入門者向けの Symfony プロジェクトファイル一式で、これをダウンロードすればすぐに開発を始められます。

.. tip::

    ソースコード管理システムの元でプロジェクトを新規作成し、ソースコード管理システムに登録するベストプラクティスを知りたい方は、この章の\ `ソースコード管理システムで管理するには`_\ の節を参照してください。

Symfony2 ディストリビューションのダウンロード
---------------------------------------------

.. tip::

    まずはじめに、PHP 5.3.8 以上が動作する Web サーバ(Apache など)がインストールされ、設定済みかどうか確認して下さい。
    Symfony2 の動作要件について詳しくは、\ :doc:`Symfony2 の動作に必要な要件</reference/requirements>` を参照してください。

Symfony2 のディストリビューションは完全に動作するアプリケーションです。Symfony2 コアライブラリとコアバンドルが実用的なディレクトリ構造に格納され、いくつかの初期設定が含まれています。
Symfony2 のディストリビューションをダウンロードすることは、
アプリケーションをすぐに開発し始められる実用的なアプリケーションのスケルトンをダウンロードしている事にもなります。

はじめに Symfony2 ダウンロードページ(\ `http://symfony.com/download`_\ )にアクセスしてください。
このページから Symfony2 のメインのディストリビューションである *Symfony Standard Edition* をダウンロードできます。このディストリビューションを使ってプロジェクトを開始する方法を説明します。

その1) Composer
~~~~~~~~~~~~~~~

`Composer`_ は PHP の依存パッケージ管理ライブラリです。Composer を使って Symfony2 Standard Edition をダウンロードできます。

ローカルコンピュータにまだ Composer を準備していない場合は、\ `Composer をダウンロード`_ してください。curl がインストールされていれば、次のコマンドだけでインストールできます。

.. code-block:: bash

    $ curl -s https://getcomposer.org/installer | php

.. note::

    ローカルコンピュータで Composer を使う環境が整っていない場合は、上記コマンドの実行時にいくつかの指示が表示されます。Composer を適切に動作させるためには、表示された指示にしたがって環境を整えてください。

Composer は実行可能な PHAR ファイルです。Composer を使って次のコマンドで Symfony Standard ディストリビューションをダウンロードします:

.. code-block:: bash

    $ php composer.phar create-project symfony/framework-standard-edition /path/to/webroot/Symfony dev-master

.. tip::

    ダウンロードするバージョンを明示的に指定するには、\ `dev-master` の部分を最新の Symfony のバージョン(2.1.1 など)に置き換えてください。詳細は\ `Symfony のダウンロードページ`_ 参照してください。

.. tip::

    Tests などの不要なディレクトリを除外してベンダーファイル群のダウンロード時間を短縮するには、Composer コマンドの末尾に ``--prefer-dist`` オプションを追加してください。

このコマンドにより、Standard ディストリビューションと必要なすべてのベンダーライブラリが Composer によりダウンロードされるので、完了までしばらく時間がかかります。コマンドが完了すると、次のようなディレクトリ構造ができています:

.. code-block:: text

    path/to/webroot/ <- web ルートディレクトリ
        Symfony/ <- 作成されたディレクトリ
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

その2) アーカイブをダウンロード
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Standard Edition のアーカイブをダウンロードすることもできます。次の 2 つの選択肢に対応したアーカイブがあります:

* ``.tgz`` 形式、もしくは \ ``.zip`` 形式の選択 - どちらも中身は同じですので、使いやすい方を選んでください。

* ディストリビューションにベンダーファイル群を含めるか含めないか。
  サードパーティのライブラリやバンドルを追加し、それらを Composer で管理したい場合は、"without vendors(ベンダーライブラリなし)" を選択してください。

いずれかのアーカイブをダウンロードし、ローカル Web サーバーの公開ディレクトリ配下などへ展開してください。
UNIX のコマンドラインであれば、以下のコマンドのどちらかを実行するとアーカイブを展開できます(\ ``###`` 部分は実際のファイル名に置き換えて実行してください\ )。

.. code-block:: bash

    # .tgz ファイル用
    $ tar zxvf Symfony_Standard_Vendors_2.1.###.tgz

    # .zip ファイル用
    $ unzip Symfony_Standard_Vendors_2.1.###.zip

"without vendors" のアーカイブをダウンロードした場合は、次に説明するベンダーの更新も行なってください。

.. note::

    デフォルトのディレクトリ構造を変更することもできます。
    詳細は :doc:`/cookbook/configuration/override_dir_structure` を参照してください。

.. _installation-updating-vendors:

ベンダーの更新
~~~~~~~~~~~~~~

ここまでの手順で、完全に機能する 1 つの Symfony プロジェクトのダウンロードが完了しました。
このプロジェクトを起点としてアプリケーション開発を開始できます。Symfony プロジェクトは、多くの外部ライブラリを利用しています。
外部ライブラリは `Composer`_ により `vendor/` ディレクトリへダウンロードされます。

Symfony プロジェクトのダウンロード方法に応じて、ここでベンダーライブラリの更新を行なってください。
ベンダーライブラリの更新は安全で、必要なベンダーライブラリがすべて揃っていることが保証されます。

ステップ 1: PHP のパッケージングシステムである `Composer`_ を入手する

.. code-block:: bash

    $ curl -s http://getcomposer.org/installer | php

``composer.phar`` をダウンロードしたディレクトリ ``composer.json`` ファイルが存在することを確認してください(デフォルトでは、ここが Symfony プロジェクトルートです)。

ステップ 2: ベンダーのインストール

.. code-block:: bash

    $ php composer.phar install

このコマンドを実行すると、Symfony 本体を含む必要なベンダーライブラリがすべてダウンロードされ、\ ``vendor/`` ディレクトリへ格納されます。

.. note::

    お使いの環境に ``curl`` がインストールされていない場合は、http://getcomposer.org/installer から ``installer`` ファイルを手動でダウンロードしてください。
    このファイルをプロジェクトルートへ配置し、次のコマンドを実行してください:

    .. code-block:: bash

        $ php installer
        $ php composer.phar install

.. tip::

    ``php composer.phar install`` コマンド、または ``php composer.phar update`` コマンドを実行すると、Composer によりインストール後(post install)コマンド、または更新後(post update)コマンドが実行されます。これらのコマンドによりキャッシュのクリアやアセットのインストールが行われます。
    デフォルトで、アセットは ``web`` ディレクトリへコピーされます。
    アセットのコピーではなくシンボリックリンクを作成したい場合は、次のように composer.json ファイルの ``extra`` ノードにキーが ``symfony-asseets-install``\ 、値が ``symlink`` のノードを追加してください:

    .. code-block:: text

        "extra": {
            "symfony-app-dir": "app",
            "symfony-web-dir": "web",
            "symfony-assets-install": "symlink"
        }

    symfony-assets-install に ``symlink`` の代わりに ``relative`` を指定すると、コマンドにより相対シンボリックリンクが作成されます。

設定とセットアップ
~~~~~~~~~~~~~~~~~~

ここまでの手順で、必要なサードパーティライブラリのすべてが ``vendor/`` ディレクトリに存在します。
また標準のアプリケーションが ``app/`` ディレクトリにセットアップされ、
いくつかのサンプルコードが ``src/`` ディレクトリの中にあります。

Symfony2 には、Web ブラウザからアクセスできる設定テスターが同梱されています。この設定テスターを使って、Web サーバーと PHP が Symfony を使えるよう設定されているかを確認できます。以下の URL で設定テスターにアクセスします。

.. code-block:: text

    http://localhost/config.php

設定テスターで問題が表示された場合は、この段階で修正しておくことをおすすめします。

.. sidebar:: パーミッションの設定

    よくある問題としては、 ``app/cache`` と ``app/logs`` ディレクトリが、Web サーバーの実行ユーザーとコマンドラインの実行ユーザーのいずれからも書き込み可能でなければならないことです。
    UNIX システム上で Web サーバーのユーザーとコマンドラインユーザーが異なる場合は、以下のコマンドをプロジェクト内で1度実行するだけで、パーミッションを適切にセットアップできます。

    **Web サーバーの実行ユーザーを確認する**
    
    以降の例では Web サーバーの実行ユーザーが ``www-data`` として説明していますが、異なるユーザーを利用する Web サーバーもあります。
    お使いの環境の Web サーバーの実行ユーザーを確認し、\ ``www-data`` の代わりに指定してください。

    UNIX システムでは、次のようなコマンドで確認できます。

    .. code-block:: bash

        $ ps aux | grep httpd

    または

    .. code-block:: bash

        $ ps aux | grep apache

    **1. chmod +a コマンドをサポートしているシステム上で ACL を使う**

    多くのシステムでは ``chmod +a`` コマンドが使えます。
    パーミッションの設定には、最初にこのコマンドを試してください。
    コマンドがエラーになった場合は、2 の方法を試してください。
    1 つめの ``chmod`` コマンドで指定している ``www-data`` は、お使いの Web サーバーの実行ユーザーに置き換えてください。

    .. code-block:: bash

        $ rm -rf app/cache/*
        $ rm -rf app/logs/*

        $ sudo chmod +a "www-data allow delete,write,append,file_inherit,directory_inherit" app/cache app/logs
        $ sudo chmod +a "`whoami` allow delete,write,append,file_inherit,directory_inherit" app/cache app/logs

    **2. chmod +a コマンドをサポートしていないシステム上で ACL を使う**

    ``chmod +a`` コマンドがサポートされていないシステムもあります。
    このようなシステムでも ``setfacl`` ユーティリティがサポートされているかもしれません。
    たとえば Ubuntu であれば、まず setfacl ユーティリティをインストールし、使用しているパーティションに対して `ACL サポートを有効にする`_ 設定を行ってください。

    .. code-block:: bash

        $ sudo setfacl -R -m u:www-data:rwX -m u:`whoami`:rwX app/cache app/logs
        $ sudo setfacl -dR -m u:www-data:rwx -m u:`whoami`:rwx app/cache app/logs

    **3. ACL を使わない方法**

    もしディレクトリの ACL を変更するためのアクセス権がなければ、umask を変更して対応します。
    この場合、cache ディレクトリと log ディレクトリには、グループ書き込み権限か全てのユーザー書き込み権限
    (Web サーバーのユーザーとコマンドラインユーザーが同じグループかどうかに依存する)が必要になります。

    umask の変更を有効にするには、以下の行を ``app/console``\ 、\ ``web/app.php``\ 、\ ``web/app_dev.php`` ファイルの先頭に記述します。

    .. code-block:: php

        umask(0002); // パーミッションを 0775 に設定します

        // or

        umask(0000); // パーミッションを 0777 に設定します

    umask の変更はスレッドセーフではないため、ACL で設定可能な場合は ACL を使うことをおすすめします。

すべて設定したら、「Go to the Welcome page(ウェルカムページへ行く)」をクリックして、最初の「リアルな」\ Symfony2 の Web ページをリクエストしましょう。

.. code-block:: text

    http://localhost/app_dev.php/

Symfony2 のウェルカム画面が表示されます。

.. image:: /images/quick_tour/welcome.jpg

.. tip::
    
    アプリケーションで短いきれいな URL を使うには、Web サーバーまたはバーチャルホストのドキュメントルートを ``Symfony/web/`` ディレクトリに設定してください。
    この設定は開発段階では必須ではありませんが、運用環境ではシステムのソースコードや設定ファイルへ Web 経由でアクセスすることを防ぐ意味でも、この設定を行なっておくことをおすすめします。
    Web サーバーのドキュメントルートを設定する方法については、各 Web サーバーのドキュメントを参照してください。 `Apache`_ | `Nginx`_

開発を始める
------------

これで完全に機能する Symfony2 アプリケーションのセットアップが完了しましたので、開発を始められます。ディストリビューションの中にはいくつかのサンプルコードが含まれています。
``README.rst`` ファイルを(テキストファイルとして開いて)確認し、
どんなサンプルコードが含まれていて、後でどうやってそのサンプルコードを削除するかを学んでください。

もし Symfony での開発が初めてであれば、\ ":doc:`page_creation`" へ進んでください。
このページでは、新しくアプリケーションを開発するために最初に必要となるページの作り方や設定の変更方法について説明しています。

.. note::

    ディストリビューションからサンプルコードを削除したい場合は、クックブックの記事 ":doc:`/cookbook/bundles/remove`" を参照してください。

ソースコード管理システムで管理するには
--------------------------------------

``Git`` や ``Subversion`` のようなバージョン管理システムを使っている場合は
バージョン管理システムをセットアップして、いつも通りにプロジェクトのコミットを始めることができます。
Symfony Standard Edition は、新しいプロジェクトを開始する起点として使うことができます。

Git を使ったプロジェクトのセットアップ手順の詳細は、\ :doc:`/cookbook/workflow/new_project_git` を参照してください。


``vendor/`` ディレクトリを除外する
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*without vendors* アーカイブをダウンロードした場合、\ ``vendor/`` ディレクトリ全体がソースコード管理対象から除外されるよう設定し、ソースコード管理システムにコミットされないようにできます。
``Git`` を使っている場合は、次の内容で ``.gitignore`` ファイルを作成して追加するだけで、除外設定が完了します。

.. code-block:: text

    vendor/

これで vendor ディレクトリはソースコード管理システムにコミットされなくなったでしょう。
他の誰かがプロジェクトをクローンしたりチェックアウトする時に、その人は必要なベンダーライブラリ全てをダウンロードするために ``php composer.phar install`` というスクリプトを実行するだけです。


.. _`ACL サポートを有効にする`: https://help.ubuntu.com/community/FilePermissions#ACLs
.. _`http://symfony.com/download`: http://symfony.com/download
.. _`Git`: http://git-scm.com/
.. _`GitHub Bootcamp`: http://help.github.com/set-up-git-redirect
.. _`Composer`: http://getcomposer.org/
.. _`Composer をダウンロード`: http://getcomposer.org/download/
.. _`Apache`: http://httpd.apache.org/docs/current/mod/core.html#documentroot
.. _`Nginx`: http://wiki.nginx.org/Symfony
.. _`Symfony のダウンロードページ`:    http://symfony.com/download

.. 2011/07/23 uechoco 9de84d1fcc3fb0f641efa5b36973ab95cddf5faa
.. 2011/08/14 hidenorigoto b21a16f5196fae0d0f1f0a20d69777ea0e685911
.. 2013/03/16 hidenorigoto 5246f51f550db504e76c98b641e3337570e84dd4
