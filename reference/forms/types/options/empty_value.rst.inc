empty_value
~~~~~~~~~~~

**データ型**: ``string`` or ``Boolean``

.. versionadded:: 2.3
    Symfony 2.3 から、 ``expanded`` オプションが ``true`` の場合でも空の値がサポートされました。

このオプションは特別な「空の」選択肢(例 "選択肢を選んで下さい" )がセレクトウィジェットの一番上に表示されるかどうかを決定します。このオプションは ``multiple`` オプションが ``false`` の場合にのみ適用されます。

* テキストとして"選択肢を選んで下さい"との空の値を追加します。::

    $builder->add('states', 'choice', array(
        'empty_value' => '選択肢を選んで下さい',
    ));

* 「空の」値の選択肢が表示されないことを保証します。::

    $builder->add('states', 'choice', array(
        'empty_value' => false,
    ));

``empty_value`` を設定しないままで、 ``required`` オプションが ``false`` の場合にのみ空の選択肢（テキスト無し）が自動的に追加されます。::

    // 空の選択肢（テキスト無し）が追加されます
    $builder->add('states', 'choice', array(
        'required' => false,
    ));

.. 2013/12/28 yositani2002 ce74b62d4d5ff44e89de6a34f2df8facfb453c97
