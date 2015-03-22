.. note::

 * 対象バージョン：2.7
 * 翻訳更新日：2015/03/22

.. index::
   single: Bundle; Installation

サードパーティ製のバンドルを使用する
================================

インストール手順はバンドルにより異なりますが、概ね以下の手順で実施します。

* `A) Composer の導入`_
* `B) バンドルの有効化`_
* `C) バンドルの設定`_

A) Composerの導入
----------------------------

依存関係は Composer により管理しますので、初めての方は `Composer のドキュメント`_ を参照してください。
Composer を導入後、以下の手順を実施します。

1) Packagist から利用するバンドル名を見つける
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

基本的にはバンドルの README にバンドル名が記載されていますが(例： `FOSUserBundle`_)、見つからない場合は `Packagist.org`_ 等から検索することもできます。

.. note::

    Symfony のバンドルをお探しの方は、`KnpBundles.com`_: も参考にしてみてください。

2) Composer でバンドルをインストールする
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

バンドル名がわかったら、Composer でバンドルをインストールします。

.. code-block:: bash

    $ composer require friendsofsymfony/user-bundle

コマンドを実行することで、適用するプロジェクトに最適なバージョンのバンドルを ``composer.json`` に追加し、
``vendor/`` ディレクトリ配下にダウンロードします。
特定のバージョンが必要な場合は、ライブラリ名の後ろに ``:`` とバージョンを記述してください。
(参考: `composer require`_ )

B) バンドルの有効化
--------------------

ここまでの手順で、 Symfony プロジェクトにバンドルがインストールされて( ``vendor/friendsofsymfony/`` 配下)、オートローダーに認識されました。
あとは以下のように ``AppKernel``: にバンドルを登録するだけで、バンドルを利用できます。

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        // ...

        public function registerBundles()
        {
            $bundles = array(
                // ...,
                new FOS\UserBundle\FOSUserBundle(),
            );

            // ...
        }
    }

C) バンドルの設定
-----------------------

ほとんどのバンドルでは、以上の手順に加えて ``app/config/config.yml`` になんらかの設定を追加する場合があります。
必要な設定については、各バンドルのドキュメントを参照するか、 ``config:dump-reference`` コンソールコマンドから参照します。

例えば、``Assetic`` の設定は,

.. code-block:: bash

    $ app/console config:dump-reference AsseticBundle

または、

.. code-block:: bash

    $ app/console config:dump-reference assetic

を実行することで、以下のように確認することができます。

.. code-block:: text

    assetic:
        debug:                %kernel.debug%
        use_controller:
            enabled:              %kernel.debug%
            profiler:             false
        read_from:            %kernel.root_dir%/../web
        write_to:             %assetic.read_from%
        java:                 /usr/bin/java
        node:                 /usr/local/bin/node
        node_paths:           []
        # ...

その他の設定
-----------

その他の設定については、利用するバンドルのREADMEファイルを参照してください。
快適なバンドルライフをお楽しみください！

.. _Composer のドキュメント: http://getcomposer.org/doc/00-intro.md
.. _Packagist.org:       https://packagist.org
.. _FOSUserBundle:       https://github.com/FriendsOfSymfony/FOSUserBundle
.. _KnpBundles.com:      http://knpbundles.com/
.. _`composer require`:  https://getcomposer.org/doc/03-cli.md#require

.. 2015/03/22 hanahiroAze 9ab1c56caddbbf7c7de3493937671cfdddac6d4e
