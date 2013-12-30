.. index::
   single: Forms; Fields; choice

.. note::

   * 対象バージョン：2.4 (2.0以降)
   * 翻訳更新日：2013/12/30

choice フィールドタイプ
=======================

多目的フィールドはユーザーが一つ以上の選択肢を"選択"することができるようにするために使用します。
``select`` タグ、 ``radio`` ボタン、または ``checkbox`` として表示されます。

このフィールドを使用するには、``choice_list`` または ``choices`` オプションの *どちらか* を指定する必要があります。

+-------------+------------------------------------------------------------------------------+
| 対応するタグ| さまざまなタグに対応 (下記参照)                                              |
+-------------+------------------------------------------------------------------------------+
| オプション  | - `choices`_                                                                 |
|             | - `choice_list`_                                                             |
|             | - `multiple`_                                                                |
|             | - `expanded`_                                                                |
|             | - `preferred_choices`_                                                       |
|             | - `empty_value`_                                                             |
+-------------+------------------------------------------------------------------------------+
| 継承された  | - `required`_                                                                |
| オプション  | - `label`_                                                                   |
|             | - `read_only`_                                                               |
|             | - `disabled`_                                                                |
|             | - `error_bubbling`_                                                          |
|             | - `error_mapping`_                                                           |
|             | - `mapped`_                                                                  |
|             | - `inherit_data`_                                                            |
|             | - `by_reference`_                                                            |
|             | - `empty_data`_                                                              |
+-------------+------------------------------------------------------------------------------+
| 親タイプ    | :doc:`form </reference/forms/types/form>`                                    |
+-------------+------------------------------------------------------------------------------+
| クラス      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\ChoiceType`         |
+-------------+------------------------------------------------------------------------------+

利用例
------

このフィールドを使用する最も簡単な方法は、 ``choices`` オプションで選択肢を直接指定することです。
配列のキーは実際にセットされる基礎となるオブジェクトの値（例えば ``m`` ）となり、一方、配列の値（例えば ``Male`` ）はフォーム上で表示されます。

.. code-block:: php

    $builder->add('gender', 'choice', array(
        'choices'   => array('m' => 'Male', 'f' => 'Female'),
        'required'  => false,
    ));

``multiple`` を ``true`` に設定することで、複数の値を選択することが出来ます。ウィジェットが複数選択の ``select`` タグとして表示されるか、一連の ``checkbox`` として表示するかは ``expanded`` オプションによります。:

.. code-block:: php

    $builder->add('availability', 'choice', array(
        'choices'   => array(
            'morning'   => 'Morning',
            'afternoon' => 'Afternoon',
            'evening'   => 'Evening',
        ),
        'multiple'  => true,
    ));

``choice_list`` オプションを使うことで、一つのオブジェクトでウィジェットの選択肢を指定できます。

.. _forms-reference-choice-tags:

.. include:: /reference/forms/types/options/select_how_rendered.rst.inc

フィールドオプション
--------------------

choices
~~~~~~~

**データ型**: ``array`` **デフォルト**: ``array()``

このオプションはこのフィールドで使用されるべき選択肢を特定する最も基本的な方法です。 ``choices`` オプションは一つの配列で、配列のキーはアイテムの値であり、配列の値はアイテムのラベルです。::

    $builder->add('gender', 'choice', array(
        'choices' => array('m' => 'Male', 'f' => 'Female')
    ));

choice_list
~~~~~~~~~~~

**データ型**: ``Symfony\Component\Form\Extension\Core\ChoiceList\ChoiceListInterface``

これはこのフィールドで使われるオプションを特定する一つの方法です。
``choice_list`` オプションは ``ChoiceListInterface`` のインスタンスでなければなりません。
より高度な例として、選択肢を供給するためそのインターフェイスを実装したカスタムクラスを作成することができます。

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

継承されたオプション
--------------------

以下のオプションは :doc:`form </reference/forms/types/form>` タイプを継承しています:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/inherit_data.rst.inc

.. include:: /reference/forms/types/options/by_reference.rst.inc

.. include:: /reference/forms/types/options/empty_data.rst.inc

.. 2013/12/30 yositani2002 b675661199d466be4b6cb6f70d16aa1e3574c789
