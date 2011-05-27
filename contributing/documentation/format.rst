.. 2011/05/08 doublemarket e9e057b3

ドキュメントのフォーマット
===========================

Symfony2 のドキュメントはマークアップ言語として `reStructuredText`_ を使っており、\ `Sphinx`_ を使って HTML、PDF などのドキュメントを出力しています。

reStructuredText
----------------

reStructuredText は、「読みやすく、見たままのプレーンテキストマークアップ構文をパースするシステム」です。

構文について詳しく学びたい方は、Symfony2 の\ `ドキュメント`_\ のソースを読むか、Sphinx Web サイトの\ `reStructuredText Primer`_\ を読んでください。

Markdown フォーマットを扱ったことがあるなら、次のような似ているが異なる構文に注意してください。

* リストは行の先頭で開始します (インデントはできません)

* インラインのコードブロックには 2 重のバッククォートを使います (````このように````)。

Sphinx
------

Sphinx は、reStructuredText ファイルからドキュメントを生成するビルドシステムで、いくつかのすばらしい機能があります。
たとえば、標準的な、reST `マークアップ`_\ のほかに、ディレクディブやテキストロールが追加されています。

構文のハイライト
~~~~~~~~~~~~~~~~

すべてのサンプルコードは、ハイライトされるデフォルトの言語として PHP を使います。
このデフォルト設定は、次のように ``code-block`` ディレクティブで変更できます。

.. code-block:: rst

    .. code-block:: yaml

        { foo: bar, bar: { foo: bar, bar: baz } }

PHP コードが ``<?php`` で始まる場合、ハイライト用の擬似言語として ``html+php`` を使ってください。

.. code-block:: rst

    .. code-block:: html+php

        <?php echo $this->foobar(); ?>

.. note::

   ハイライトがサポートされている言語の一覧は、 `Pygments website`_ をご確認ください。

設定ブロック
~~~~~~~~~~~~

設定を表示する場合は ``configuration-block`` ディレクティブを使い、サポートされているすべての設定フォーマット (``PHP``\ 、\ ``YAML``\ 、\ ``XML``) を表示できるようにします。

.. code-block:: rst

    .. configuration-block::

        .. code-block:: yaml

            # YAML での設定

        .. code-block:: xml

            <!-- XML での設定 //-->

        .. code-block:: php

            // PHP での設定

この reST スニペットは、次のようにレンダリングされます。

.. configuration-block::

    .. code-block:: yaml

        # YAML での設定

    .. code-block:: xml

        <!-- XML での設定 //-->

    .. code-block:: php

        // PHP での設定

現在サポートされているフォーマットの一覧は以下のとおりです。

+--------------------------+-------------+
| マークアップフォーマット | 表示        |
+==========================+=============+
| html                     | HTML        |
| xml                      | XML         |
| php                      | PHP         |
| yaml                     | YAML        |
| jinja                    | Twig        |
| html+jinja               | Twig        |
| jinja+html               | Twig        |
| php+html                 | PHP         |
| html+php                 | PHP         |
| ini                      | INI         |
| php-annotations          | Annotations |
+--------------------------+-------------+

ドキュメントのテスト
~~~~~~~~~~~~~~~~~~~~

コミット前にドキュメントをテストするには、次の手順に従います。

* `Sphinx`_; をインストールします。

* `Sphinx quick setup`_; を実行します。

* 設定ブロックの Sphinx エクステンションをインストールします (下の項を参照)。

* ``make html`` を実行し、 ``build`` ディレクトリ内に生成された HTML を確認します。

設定ブロックの Sphinx エクステンションのインストール
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* `configuration-block source`_ リポジトリからエクステンションをダウンロードします。

* ``configurationblock.py`` をソースフォルダ内の ``_exts`` にコピーします
  (``conf.py`` が置かれているフォルダです) 。

* ``conf.py`` ファイルに以下を追加します。

.. code-block:: py
    
    # ...
    sys.path.append(os.path.abspath('_exts'))
    
    # ...
    # エクステンションのリストに configurationblock を追加
    extensions = ['configurationblock']

.. _reStructuredText:           http://docutils.sf.net/rst.html
.. _Sphinx:                     http://sphinx.pocoo.org/
.. _ドキュメント:               http://github.com/symfony/symfony-docs
.. _reStructuredText Primer:    http://sphinx.pocoo.org/rest.html
.. _マークアップ:               http://sphinx.pocoo.org/markup/
.. _Pygments website:           http://pygments.org/languages/
.. _configuration-block source: https://github.com/fabpot/sphinx-php
.. _Sphinx quick setup:         http://sphinx.pocoo.org/tutorial.html#setting-up-the-documentation-sources
