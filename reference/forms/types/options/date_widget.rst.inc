widget
~~~~~~

**データ型**: ``string`` **デフォルト**: ``choice``

フィールドの表示の基本的な方法です。次のいずれかになります。
The basic way in which this field should be rendered. Can be one of the following:

* ``choice``: 3つの選択フォームを表示します。選択の順序は、`format`_ オプションで定義されています。

* ``text``: 3つの ``text`` タイプの入力フィールド（月、日、年）をレンダリングします。

* ``single_text``: ``date`` タイプの単一の入力フォームをレンダリングします。ユーザーの入力は `format`_ オプションに基づいて検証されます。

.. 2014/05/05 yositani2002 8d4f444c25ba45439bef046bda1b1abb89c84ed0
