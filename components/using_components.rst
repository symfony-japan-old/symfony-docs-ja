.. index::
   single: Components; Installation
   single: Components; Usage

.. note::

    * 翻訳更新日：2013/04/09

Symfony2 Componentsをインストールして使う方法
==============================================

新しいプロジェクトを始めるとき（あるいは既にプロジェクトを進行中のとき）に1つ以上のコンポーネントを使うなら、一番簡単な方法は全てを `Composer`_ で管理することです。
Composerは必要なコンポーネントをダウンロードし、オートロードを手配してくれるため、すぐにライブラリを使い始めることができます。

この記事では :doc:`/components/finder` コンポーネントを例として話を進めますが、どのコンポーネントにも使用することができます。

Finderコンポーネントを使う
--------------------------

**1.** 新しいプロジェクトを始めようとしているときは、プロジェクト用に新しく空のディレクトリを作成してください。

**2.** ``composer.json`` という新しい空のファイルを作成し、下記をコピーペーストしてください。

.. code-block:: json

    {
        "require": {
            "symfony/finder": "2.2.*"
        }
    }

既に ``composer.json`` ファイルがあるときは、この行を追加するだけで良いです。必要に応じてバージョンを調整することができます。(例えば ``2.1.1`` や ``2.2.*``)

コンポーネントの名称とバージョンは `packagist.org`_ で調べることができます。

**3.** もしまだインストールしていなければ、 `composerをインストール`_ してください。

**4.** ベンダーライブラリをダウンロードし、 ``vendor/autoload.php`` ファイルを自動生成します。

.. code-block:: bash

    $ php composer.phar install

**5.** 自分のコードを書き始めて下さい。

Composerがコンポーネントをダウンロードしてしまえば、必要なのはComposerが自動で生成した ``vendor/autoload.php`` ファイルをインクルードすることだけです。 このファイルは全てのライブラリについてオートロードを全て手配してくれるため、すぐにライブラリを使い始めることができます。
::

        // File: src/script.php

        // パスはこのファイルから "vendor/" ディレクトリへの相対パスに変更してください
        require_once '../vendor/autoload.php';

        use Symfony\Component\Finder\Finder;

        $finder = new Finder();
        $finder->in('../data/');

        // ...

.. tip::

    もしSymfony2 Componentsを全て使いたいときは、下記のようにコンポーネント名を一つずつ書いていくのではなく、

        .. code-block:: json

            {
                "require": {
                    "symfony/finder": "2.2.*",
                    "symfony/dom-crawler": "2.2.*",
                    "symfony/css-selector": "2.2.*"
                }
            }

   下記のように書くこともできます。

        .. code-block:: json

            {
                "require": {
                    "symfony/symfony": "2.2.*"
                }
            }

    但し、この書き方だと全てのBundleとBridgeのライブラリも含まれてしまいます。BundleとBridgeのライブラリは実際には不要かもしれませんが。

それで?
---------

もうコンポーネントはインストールされてオートロードされるようになったわけですから、個別のコンポーネントのドキュメントを読み、使い方を学んでください。

そして、楽しんでください！

.. _Composer: http://getcomposer.org
.. _composerをインストール: http://getcomposer.org/download/
.. _packagist.org: https://packagist.org/

.. 2013/04/09 77web 5dc2b8b748e9f4f77f4415d79dfcfd0dd17a341e