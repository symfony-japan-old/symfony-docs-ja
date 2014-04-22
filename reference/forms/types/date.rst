.. index::
   single: Forms; Fields; date

.. note::

   * 対象バージョン：2.3+
   * 翻訳更新日：2014/04/23

date フィールドタイプ
=====================

異なった様々な HTML 要素で日付情報を変更することができるフィールドです。

このフィールドタイプに使用される元となるデータは ``DateTime`` オブジェクト、 ``string`` 、 ``timestamp`` または ``array`` です。
`input`_ オプションが正しく設定されている限り、フィールドはすべての詳細を引き受けます。

このフィールドは1つのテキストボックス、または、3つのテキストボックス（月、日、年）、または、3つのセレクトボックス（参照 `widget`_ オプション)として表示することができます。

+----------------------+-----------------------------------------------------------------------------------------+
| 対応するデータ型     | ``DateTime``,``string``, ``timestamp``, ``array``  (参照 ``input`` オプション)          |
+----------------------+-----------------------------------------------------------------------------------------+
| 対応するタグ         | 単一のテキストボックス または、 3つのセレクトフィールド                                 |
+----------------------+-----------------------------------------------------------------------------------------+
| オプション           | - `widget`_                                                                             |
|                      | - `input`_                                                                              |
|                      | - `empty_value`_                                                                        |
|                      | - `years`_                                                                              |
|                      | - `months`_                                                                             |
|                      | - `days`_                                                                               |
|                      | - `format`_                                                                             |
|                      | - `model_timezone`_                                                                     |
|                      | - `view_timezone`_                                                                      |
+----------------------+-----------------------------------------------------------------------------------------+
| 上書きされた         | - `by_reference`_                                                                       |
| オプション           | - `error_bubbling`_                                                                     |
+----------------------+-----------------------------------------------------------------------------------------+
| 継承された           | - `data`_                                                                               |
| オプション           | - `invalid_message`_                                                                    |
|                      | - `invalid_message_parameters`_                                                         |
|                      | - `read_only`_                                                                          |
|                      | - `disabled`_                                                                           |
|                      | - `mapped`_                                                                             |
|                      | - `inherit_data`_                                                                       |
|                      | - `error_mapping`_                                                                      |
+----------------------+-----------------------------------------------------------------------------------------+
| 親タイプ             | :doc:`form </reference/forms/types/form>`                                               |
+----------------------+-----------------------------------------------------------------------------------------+
| クラス               | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\DateType`                      |
+----------------------+-----------------------------------------------------------------------------------------+

基本的な使い方
--------------

このフィールドタイプは高度に設定が可能でありながら、簡単に使えます。最も重要なオプションは ``input`` と ``widget`` です。
This field type is highly configurable, but easy to use. The most important
options are ``input`` and ``widget``.

``publishedAt`` フィールドに``DateTime`` オブジェクトの日付データを持つとします。
以下は、``date`` タイプを3つの選択フィールドとして設定します。:

.. code-block:: php

    $builder->add('publishedAt', 'date', array(
        'input'  => 'datetime',
        'widget' => 'choice',
    ));

この ``input`` オプションは元となる日付データと同じタイプに変換され *なくてはいけません* 。
例えば、 ``publishedAt`` フィールドデータが Unix タイムスタンプの場合、``input`` に ``timestamp`` を設定しないといけません。:

.. code-block:: php

    $builder->add('publishedAt', 'date', array(
        'input'  => 'timestamp',
        'widget' => 'choice',
    ));

フィールドは ``array`` と ``string`` を正しい ``input`` オプションの値としてサポートします。
The field also supports an ``array`` and ``string`` as valid ``input`` option
values.

フィールドオプション
--------------------

.. include:: /reference/forms/types/options/date_widget.rst.inc

.. _form-reference-date-input:

.. include:: /reference/forms/types/options/date_input.rst.inc

empty_value
~~~~~~~~~~~

**データ型**: ``string`` または ``array``

もし、ウィジェットのオプションが ``choice`` がセットされていた場合、一連のセレクトボックスとして表現されます。
``empty_value`` オプションは"空"エントリーを各セレクトボックスの一番上に追加できます。::

    $builder->add('dueDate', 'date', array(
        'empty_value' => '',
    ));

別の方法として、"空"の値として特定の文字を指定することが出来ます。::

    $builder->add('dueDate', 'date', array(
        'empty_value' => array('year' => 'Year', 'month' => 'Month', 'day' => 'Day')
    ));

.. include:: /reference/forms/types/options/years.rst.inc

.. include:: /reference/forms/types/options/months.rst.inc

.. include:: /reference/forms/types/options/days.rst.inc

.. _reference-forms-type-date-format:

.. include:: /reference/forms/types/options/date_format.rst.inc

.. include:: /reference/forms/types/options/model_timezone.rst.inc

.. include:: /reference/forms/types/options/view_timezone.rst.inc

上書きされたオプション
----------------------

by_reference
~~~~~~~~~~~~

**デフォルト**: ``false``

``DateTime`` クラスは不変のオブジェクトとして扱われます。

error_bubbling
~~~~~~~~~~~~~~

**デフォルト**: ``false``

継承されたオプション
--------------------

以下のオプションは :doc:`form </reference/forms/types/form>` タイプを継承しています:

.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/inherit_data.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

フィールド変数
--------------

+--------------+------------+---------------------------------------------------------------------------------+
| 変数         | データ型   | 使用法                                                                          |
+==============+============+=================================================================================+
| widget       | ``mixed``  | `widget`_ オプションの値。                                                      |
+--------------+------------+---------------------------------------------------------------------------------+
| type         | ``string`` | ``widget`` が ``single_text`` で、且つ、 HTML5 が有効な場合のみ、               |
|              |            |  インプットタイプ(``datetime``, ``date``  または ``time`` )を使用できます。     |
+--------------+------------+---------------------------------------------------------------------------------+
| date_pattern | ``string`` | 日付フォーマットの文字列                                                        |
+--------------+------------+---------------------------------------------------------------------------------+

.. 2014/04/23 yositani200 d49d12eaf265a5d6d32ac660c62f385d57261475
