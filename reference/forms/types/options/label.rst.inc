label
~~~~~

**データ型**: ``string`` **デフォルト**: フィールド名から推測されたものです。

フィールドがレンダリングされる際にラベルが設定されます。\ ``false`` が設定されるとラベルは表示されません。
ラベルはテンプレート内部で設定することもできます。

.. code-block:: jinja

    {{ render_label(form.name, 'Your name') }}

.. 2013/11/23 nntsugu 3dd42a4809ecf3b34beb219acafcd4c62907c785
