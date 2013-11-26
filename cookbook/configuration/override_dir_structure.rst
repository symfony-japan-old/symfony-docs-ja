.. index::
   single: Override Symfony

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/11/26


Symfony のディレクトリ構造をカスタマイズする方法
=====================================================

Symfony には最初からデフォルトのディレクトリ構造があります。このディレクトリ構造は開発者が好きなように変更することができます。
デフォルトのディレクトリ構造は下記のようになっています。

.. code-block:: text

    app/
        cache/
        config/
        logs/
        ...
    src/
        ...
    vendor/
        ...
    web/
        app.php
        ...

.. _override-cache-dir:

``cache`` ディレクトリを変更する
--------------------------------

キャッシュのディレクトリは、アプリケーションの ``AppKernel`` クラスの ``getCacheDir`` をオーバーライドすることにより変更できます。::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        // ...

        public function getCacheDir()
        {
            return $this->rootDir.'/'.$this->environment.'/cache';
        }
    }

``$this->rootDir`` は ``app`` ディレクトリへの絶対パスで、 ``$this->environment`` は現在の環境です（例えば、 ``dev`` のような）
上の例では、キャッシュのディレクトリを ``app/{environment}/cache`` に変更しています。

.. caution::

    ``cache`` ディレクトリは各環境ごとに別々にしておかなくてはいけません。さもないと、予想できない挙動となることがあります。
    各環境にはそれぞれの設定ファイルのキャッシュを生成するため、キャッシュファイルを保存するために他とは違うディレクトリが必要なのです。

.. _override-logs-dir:

``logs`` ディレクトリを変更する
-------------------------------

``logs`` ディレクトリを変更する方法は、 ``cache`` ディレクトリを変更するのと同じですが、オーバーライドするメソッドは ``getLogDir`` です。::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        // ...

        public function getLogDir()
        {
            return $this->rootDir.'/'.$this->environment.'/logs';
        }
    }

上の例ではディレクトリのパスを ``app/{environment}/logs`` に変更しています。

``web`` ディレクトリを変更する
------------------------------

``web`` ディレクトリのパスを変更したり移動したりする必要があるなら、フロントコントローラーである ``app.php`` と ``app_dev.php`` にある ``app`` ディレクトリのパスが正しく保たれているだけで大丈夫です。
単に web ディレクトリのディレクトリ名を変更しただけであれば、そのままで構いません。しかし、もしどこかに移動したのなら、フロントコントローラーに含まれるパスを変更しなければなりません。::

    require_once __DIR__.'/../Symfony/app/bootstrap.php.cache';
    require_once __DIR__.'/../Symfony/app/AppKernel.php';

更に、 ``composer.json`` にある ``extra.symfony-web-dir`` オプションも変更しなければならないでしょう。

.. code-block:: json

    {
        ...
        "extra": {
            ...
            "symfony-web-dir": "my_new_web_dir"
        }
    }

.. tip::

    共有ホスティングでは ``public_html`` が公開ディレクトリとして使われることがあります。 ``web`` ディレクトリを ``public_html`` にリネームするのは、共有ホスティングで Symfony プロジェクトを動かす方法の一つです。
    別の方法として、アプリケーションを公開ディレクトリ外にデプロイし、 ``public_html`` を削除して、アプリケーションの ``web`` ディレクトリへのシンボリックリンクに置き換えてしまうという方法もあります。

.. note::

    AsseticBundle を使っている場合は、バンドルが ``web`` ディレクトリを正しく使えるように下記の設定も必要です。

    .. configuration-block::

        .. code-block:: yaml

            # app/config/config.yml

            # ...
            assetic:
                # ...
                read_from: "%kernel.root_dir%/../../public_html"

        .. code-block:: xml

            <!-- app/config/config.xml -->

            <!-- ... -->
            <assetic:config read-from="%kernel.root_dir%/../../public_html" />

        .. code-block:: php

            // app/config/config.php

            // ...
            $container->loadFromExtension('assetic', array(
                // ...
                'read_from' => '%kernel.root_dir%/../../public_html',
            ));

    設定を変更したら、再度アセットをダンプするだけで、アプリケーションを使うことができます。

    .. code-block:: bash

        $ php app/console assetic:dump --env=prod --no-debug


.. 2013/12/09 77web 1f7898045f3f1ad06dc63f32f4d921519fc5e3d7