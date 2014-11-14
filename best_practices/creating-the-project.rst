.. note::

    * 対象バージョン：2.3以降
    * 翻訳更新日：2014/11/03

プロジェクトの作成
==================

Symfony のインストール
----------------------

これが Symfony をインストールする上で推奨するたったひとつの方法です。

.. best-practice::

    常に `Composer`_ で Symfony をインストールする

Composer はモダンな PHP アプリケーションで使用されている依存パッケージの管理ツールです。
プロジェクトに必要なパッケージを追加したり削除するときや
コードで使用されているサードパーティ製のライブラリを更新するときに Composer を使えばスムーズでしょう。

Composer による依存パッケージの管理
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony をインストールする前にグローバルに Composer をインストールする必要があります。
ターミナル(コマンドコンソール)を起動し以下のコマンドを実行してください。

.. code-block:: bash

    $ composer --version
    Composer version 1e27ff5e22df81e3cd0cd36e5fdd4a3c5a031f4a 2014-08-11 15:46:48

おそらく、これとは異なるバージョンが表示されるでしょうが、
これは、Composer が継続的に更新されているためですので心配いりません。
バージョンも特に気にしなくて大丈夫です。

Composer をグローバルにインストールする
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Linux か Mac OS X を利用している場合で、
Composer がグローバルにインストールしていない場合は、
以下の２つのコマンドを実行してください。
(２つ目のコマンドでは ユーザパスワードが求められます)

.. code-block:: bash

    $ curl -sS https://getcomposer.org/installer | php
    $ sudo mv composer.phar /usr/local/bin/composer

.. note::

    利用している Linux のディストリビューションによっては
    ``sudo`` の代わりに ``su`` コマンドを実行する必要があります。

もし Windows を利用している場合、
`Composer download page`_ よりインストーラーをダウンロードして
実行しインストールを進めてください。

ブログアプリケーションの作成
----------------------------

ここまで正しく設定できていれば、Symfony に基づいた新規プロジェクトを作成できます。
コマンドコンソールでファイル作成権限があるディレクトリに移動し以下のコマンドを実行してください。

.. code-block:: bash

    $ cd projects/
    $ composer create-project symfony/framework-standard-edition blog/

このコマンドは
入手できるもっとも新しい安定版の Symfony による新しいプロジェクトを ``blog`` という
新規ディレクトリをに作成します。

Symfony の動作確認
~~~~~~~~~~~~~~~~~~

インストールが完了したら ``blog/`` ディレクトリに移動し
Symfony が正しくインストールされているか以下のコマンドを実行して確認しましょう。

.. code-block:: bash

    $ cd blog/
    $ php app/console --version

    Symfony version 2.6.* - app/dev/debug

インストールした Symfony のバージョンが確認できれば、期待通りに動作しています。
確認できなければ、システムの何が Symfony アプリケーションが正しく動作することを
妨げているか以下のスクリプトで確認してください。

.. code-block:: bash

    $ php app/check.php

利用しているシステムによっては、`check.php` を実行した際に２つ以上の異なるリストが確認できます。
最初のリストでは Symfony アプリケーションを実行する際の必須なものを表示します。
次のリストでは Symfony アプリケーションの実行を最適にするためのオプションを表示します。

.. code-block:: bash

    Symfony2 Requirements Checker
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    > PHP is using the following php.ini file:
      /usr/local/zend/etc/php.ini

    > Checking Symfony requirements:
      .....E.........................W.....

    [ERROR]
    Your system is not ready to run Symfony2 projects

    Fix the following mandatory requirements
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

     * date.timezone setting must be set
       > Set the "date.timezone" setting in php.ini* (like Europe/Paris).

    Optional recommendations to improve your setup
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

     * short_open_tag should be disabled in php.ini
       > Set short_open_tag to off in php.ini*.


.. tip::

    Symfony はセキュリティの理由でデジタル署名でリリースされます。
    Symfony のインストールを全て堅実に確かめたい場合は `public check sums repository`_ と
    `these steps`_ を参照し電子署名を確認してください。

アプリケーションの構成
----------------------

アプリケーションを作成した後に、``blog/`` ディレクトリに移動すると
いくつかのファイルとディレクトリが自動的に生成されていることが確認できるでしょう。

.. code-block:: text

    blog/
    ├─ app/
    │  ├─ console
    │  ├─ cache/
    │  ├─ config/
    │  ├─ logs/
    │  └─ Resources/
    ├─ src/
    │  └─ AppBundle/
    ├─ vendor/
    └─ web/

このファイルとディレクトリの階層は、アプリケーションを構築するために Symfony に
よって推奨された慣習です。
それぞれのディレクトリの推奨された用途は以下の通りです。

* ``app/cache/``  には、アプリケーションによって生成された全てのキャッシュファイルを保管します。
* ``app/config/`` には、それぞれの環境のために定義された全ての設定を保管します。
* ``app/logs/`` には、アプリケーションによって生成された全てのログ・ファイルを保管します。
* ``app/Resources/`` には、アプリケーションのための全てのテンプレートとトランスレイションファイルを保管します。
* ``src/AppBundle/`` には、Symfony 特有のコード （各コントローラーや各ルート）、ドメインのコード（e.g 各 Doctrine クラス）、そして全てのビジネスロジックを保管します。
* ``vendor/`` には、Composer がインストールしたアプリケーションの依存パッケージを保管します。これらは決して変更するべきではありません。
* ``web/`` には、全てのフロントコントローラーファイルとスタイルシートや JavaScript ファイル、そして画像ファイルなど全ての web assets を保管します。

アプリケーションバンドル
~~~~~~~~~~~~~~~~~~~~~~~~

Symfony 2.0 がリリースされたときに、多くの開発者は論理的なモジュールにアプリケーションを分割する Symfony 1.x の
流儀を自然と採用しました。
これが、多くの Symfony アプリケーションが ``UserBundle``、``ProductBundle``、``InvoiceBundle`` などのように
論理的な機能に分割したバンドルを使用している理由です。

これはバンドルは独立したソフトウェアの一片であるかのように再利用できるとうことを意味します。
もし ``UserBundle`` が現状のままで他の Symfony アプリケーションで利用できないのであれば、
それ自体をバンドルにするべきではありません。
さらに加えると、``InvoiceBundle`` が ``ProductBundle`` に依存している場合、
２つの別々のバンドルにアドバンテージはありません。

.. best-practice::

    アプリケーションロジックのために ``AppBundle`` というただひとつのバンドルを作りなさい

ひとつの ``AppBundle`` が動作することがプロジェクトを束ねるということは
コードがより簡潔になり理解することも容易になるでしょう。
Symfony 2.6 からは、Symfony の公式ドキュメントでも ``AppBundle`` という名前を使います。

.. note::

    このアプリケーションバンドルは決して共有されることがないため、
    プロジェクトで作成した vendor の ``AppBundle`` の接頭語は必要ありません。
    （e.g AcmeAppBundle）

大体において、これらのベストプラクティスを採用する Symfony アプリケーションの
典型的なディレクトリ構成は以下のようになります。

.. code-block:: text

    blog/
    ├─ app/
    │  ├─ console
    │  ├─ cache/
    │  ├─ config/
    │  ├─ logs/
    │  └─ Resources/
    ├─ src/
    │  └─ AppBundle/
    ├─ vendor/
    └─ web/
       ├─ app.php
       └─ app_dev.php

.. tip::

    Symfony 2.6 以上のバージョンを利用している場合は、``AppBundle`` は既に生成されています。
    それよりも古いバージョンの場合は、以下のコマンドで生成することができます。

    .. code-block:: bash

        $ php app/console generate:bundle --namespace=AppBundle --dir=src --format=annotation --no-interaction

ディレクトリ構成の拡張
----------------------

プロジェクトや基盤構造で Symfony のデフォルトのディレクトリ構成からいくつかの変更が必要になった場合は、
``cache/``、``logs/``、``web/`` といったメインのディレクトリの場所を変更することができます。

さらに Symfony3 がリリースされるときにはわずかに異なるディレクトリ構成となるでしょう。

.. code-block:: text

    blog-symfony3/
    ├─ app/
    │  ├─ config/
    │  └─ Resources/
    ├─ bin/
    │  └─ console
    ├─ src/
    ├─ var/
    │  ├─ cache/
    │  └─ logs/
    ├─ vendor/
    └─ web/

これは大きな変化ですが、現時点では Symfony2 のディレクトリ構成を使うことを推奨します。

.. _`Composer`: https://getcomposer.org/
.. _`Get Started`: https://getcomposer.org/doc/00-intro.md
.. _`Composer download page`: https://getcomposer.org/download/
.. _`override the location of the main directories`: http://symfony.com/doc/current/cookbook/configuration/override_dir_structure.html
.. _`public checksums repository`: https://github.com/sensiolabs/checksums
.. _`these steps`: http://fabien.potencier.org/article/73/signing-project-releases

.. 2014/11/03 kseta 2c2000a0274b182cbf1a429badb567ee65432c54
