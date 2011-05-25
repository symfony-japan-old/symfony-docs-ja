.. 2011/05/25 doublemarket 5ba18992

ドキュメントの貢献
==================

ドキュメントは、コードと同じくらい重要です。
また、次のようなコードと同じ原則に従います:
DRY、テスト、メンテナンス性、拡張性、最適化、リファクタリングなどです。
同様に、ドキュメントにはバグ、誤字脱字、読みづらいといった問題が含まれる場合もあります。

貢献するには
------------

貢献する前に、ドキュメントで使われている\ :doc:`マークアップ言語 <format>`\ に慣れておく必要があります。

また、Symfony2 のドキュメントは GitHub でホストされています:

.. code-block:: text

   https://github.com/symfony/symfony-docs

(訳注)日本語翻訳ドキュメントは、次のリポジトリでホストされています。

.. code-block:: text

    https://github.com/symfony-japan/symfony-docs-ja.git

パッチを投稿したい場合は、 GitHub 上のオフィシャルドキュメントリポジトリを
`フォーク`_ し、それからあなたの自身のフォークにクローンしてください:

.. code-block:: bash

    $ git clone git://github.com/YOURUSERNAME/symfony-docs.git

(訳注)日本語翻訳ドキュメントの場合は、次のように clone してください。

.. code-block:: bash

    $ git clone git://github.com/YOURUSERNAME/symfony-docs-ja.git

次に、あなたの変更を含む専用のブランチを作成してください。

.. code-block:: bash

    $ git checkout -b improving_foo_and_bar

直接このブランチに変更を加え、コミットすることができます。完了したら、
このブランチを *あなたの* GitHub フォークにプッシュし、プルリクエストを
発行してください。このプルリクエストが ``improving_foo_and_bar`` ブランチと
``symfony-docs`` ``master`` ブランチの間になります。

.. image:: /images/docs-pull-request.png
   :align: center

GitHub では `プルリクエスト`_ の詳細を説明しています。

.. note::

    Symfony2 のドキュメントは Creative Commons Attribution-Share Alike 3.0
    Unported :doc:`ライセンス <license>` で公開されています。

問題を報告するには
------------------

ドキュメントに貢献する最も簡単な方法は、次のような問題を報告して頂くことです: 誤字脱字、文法上の問題、コード例の不具合、説明不足など。

手順:

* バグトラッカーへバグを送信してください

* *(任意)* パッチを投稿してください

翻訳するには
------------

翻訳に関する詳細は、\ :doc:`ドキュメント <translations>`\ のページを参照してください。

.. _`フォーク`: http://help.github.com/fork-a-repo/
.. _`プルリクエスト`: http://help.github.com/pull-requests/
