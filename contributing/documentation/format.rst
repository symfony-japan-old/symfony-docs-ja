ドキュメントのフォーマット
===========================

Symfony2 のドキュメントはマークアップ言語として `reStructuredText`_ を使っており、 `Sphinx`_ を使って HTML、PDF などのドキュメントを出力しています。

reStructuredText
----------------

reStructuredText は、「読みやすく、見たままのプレーンテキストマークアップ構文をパースするシステム」です。

構文について詳しく学びたい方は、Symfony2 の `ドキュメント`_ のソースを読むか、Sphinx Web サイトの `reStructuredText Primer`_ を読んでください。

Markdown フォーマットを扱ったことがあるなら、次のような似ているが異なる構文に注意してください:

* リストは行の先頭で開始します (インデントはできません);

* インラインのコードブロックには 2 重のバッククォートを使います (````このように````)。

Sphinx
------

Sphinx は、reStructuredText ファイルからドキュメントを生成するビルドシステムで、いくつかのすばらしい機能があります。
たとえば、標準的な、reST `マークアップ`_ 以外にディレクディブや翻訳されたテキストロールが追加されています。

構文のハイライト
~~~~~~~~~~~~~~~~

すべてのサンプルコードは、ハイライトされるデフォルトの言語として PHP を使います。
このデフォルト設定は、次のように ``code-block`` ディレクティブで変更できます:

.. code-block:: rst

    .. code-block:: yaml

        { foo: bar, bar: { foo: bar, bar: baz } }

PHP コードが ``<?php`` で始まる場合、ハイライト用の擬似言語として ``html+php`` を使ってください:

.. code-block:: rst

    .. code-block:: html+php

        <?php echo $this->foobar(); ?>

.. note::
   ハイライトがサポートされている言語の一覧は、 `Pygments website`_ をご確認ください。

設定ブロック
~~~~~~~~~~~~

設定を表示する場合は ``configuration-block`` ディレクティブを使い、サポートされているすべての設定フォーマット (``PHP`` 、 ``YAML`` 、 ``XML``) を表示できるようにします。

.. code-block:: rst

    .. configuration-block::

        .. code-block:: yaml

            # YAML での設定

        .. code-block:: xml

            <!-- XML での設定 //-->

        .. code-block:: php

            // PHP での設定

この reST スニペットは、次のようにレンダリングされます:

.. configuration-block::

    .. code-block:: yaml

        # YAML での設定

    .. code-block:: xml

        <!-- XML での設定 //-->

    .. code-block:: php

        // PHP での設定

.. _reStructuredText:        http://docutils.sf.net/rst.html
.. _Sphinx:                  http://sphinx.pocoo.org/
.. _ドキュメント:            http://github.com/symfony/symfony-docs
.. _reStructuredText Primer: http://sphinx.pocoo.org/rest.html
.. _マークアップ:            http://sphinx.pocoo.org/markup/
.. _Pygments website:        http://pygments.org/languages/
