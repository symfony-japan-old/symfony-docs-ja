.. note::

   * 対象バージョン：2.3
   * 翻訳更新日：2013/11/23

required
~~~~~~~~

**データ型**: ``Boolean`` **デフォルト**: ``true``

trueの場合は `HTML5 required attribute`_ がレンダリングされます。
対応する ``label`` もまた ``required`` クラスをレンダリングします。

これらは表面的でバリデーションからは独立しています。Symfonyに
フィールドタイプを推測することを許可する場合は、
このオプションの値はバリデーションの情報から推測されるのが理想的でしょう。

.. _`HTML5 required attribute`: http://diveintohtml5.info/forms.html

.. 2013/11/23 kseta 2d7618490739fe004d4c201835157f3a3f29cb19