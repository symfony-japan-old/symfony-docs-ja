.. index::
   single: Forms; Fields; time

.. note::

   * 対象バージョン：2.3+
   * 翻訳更新日：2014/04/23

time フィールドタイプ
=====================

時刻入力をするフィールドです。

これは1つのテキストフィールド、または、3つのテキストフィールド（例 時、分、秒）、または、一連のセレクトフィールドとして表示することができます。
元となるデータは ``DateTime`` オブジェクト、 ``string`` 、 ``timestamp`` または ``array`` として保存することが出来ます。

+----------------------+--------------------------------------------------------------------------------+
| 対応するデータ型     | ``DateTime``, ``string``, ``timestamp``, ``array`` (参照``input`` オプション ) |
+----------------------+--------------------------------------------------------------------------------+
| 対応するタグ         | 様々なタグとして利用可能 (下記参照)                                            |
+----------------------+--------------------------------------------------------------------------------+
| オプション           | - `widget`_                                                                    |
|                      | - `input`_                                                                     |
|                      | - `with_minutes`_                                                              |
|                      | - `with_seconds`_                                                              |
|                      | - `hours`_                                                                     |
|                      | - `minutes`_                                                                   |
|                      | - `seconds`_                                                                   |
|                      | - `model_timezone`_                                                            |
|                      | - `view_timezone`_                                                             |
|                      | - `empty_value`_                                                               |
+----------------------+--------------------------------------------------------------------------------+
| 上書きされた         | - `by_reference`_                                                              |
| オプション           | - `error_bubbling`_                                                            |
+----------------------+--------------------------------------------------------------------------------+
| 継承された           | - `invalid_message`_                                                           |
| オプション           | - `invalid_message_parameters`_                                                |
|                      | - `data`_                                                                      |
|                      | - `read_only`_                                                                 |
|                      | - `disabled`_                                                                  |
|                      | - `mapped`_                                                                    |
|                      | - `inherit_data`_                                                              |
|                      | - `error_mapping`_                                                             |
+----------------------+--------------------------------------------------------------------------------+
| 親タイプ             | form                                                                           |
+----------------------+--------------------------------------------------------------------------------+
| クラス               | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\TimeType`             |
+----------------------+--------------------------------------------------------------------------------+

基本的な使い方
--------------

このフィールドタイプは高度に設定が可能でありながら、簡単に使えます。最も重要なオプションは ``input`` と ``widget`` です。

``startTime`` フィールドに ``DateTime`` オブジェクトの時間データを持つとします。
以下は、``time`` タイプを2つの異なった選択フィールドとして設定します。:

.. code-block:: php

    $builder->add('startTime', 'time', array(
        'input'  => 'datetime',
        'widget' => 'choice',
    ));

この ``input`` オプションは元となる時間データと同じタイプに変換され *なくてはいけません* 。
例えば、 ``startTime`` フィールドデータが Unix タイムスタンプの場合、``input`` に ``timestamp`` を設定しないといけません。:

.. code-block:: php

    $builder->add('startTime', 'time', array(
        'input'  => 'timestamp',
        'widget' => 'choice',
    ));

フィールドは ``array`` と ``string`` を正しい ``input`` オプションの値としてサポートします。

フィールドオプション
--------------------

widget
~~~~~~

**データ型**: ``string`` **デフォルト**: ``choice``

このフィールドがレンダリングされるべき基本的な方法は次のいずれかになります。:

* ``choice``: `with_minutes`_ と `with_seconds`_ オプションに基づいて、1つ、または、2つ（デフォルト）または3つのセレクト入力（ ``hour`` , ``minute`` , ``second`` ）を表示します。

* ``text``:  `with_minutes`_ と `with_seconds`_ オプションに基づいて、1つ、または、2つ（デフォルト）または3つのテキスト入力（ ``hour`` , ``minute`` , ``second`` ）を表示します。

* ``single_text``: 単一の ``time`` タイプの入力フォームを表示します。ユーザーの入力は ``hh:mm`` （または ``HH：MM：SS`` 秒を使用している場合） のフォームに対して検証されます。

.. caution::

    ウィジェットタイプの ``single_text`` と `with_minutes`_ オプションを ``false`` とする組み合わせは、
    入力タイプ ``time``  が時間のみ選択することをサポートしていない可能性があり、
    クライアントで予期しない動作を引き起こす可能性があります。

input
~~~~~

**データ型**: ``string`` **デフォルト**: ``datetime``

*input* データのフォーマット、すなわち、対応するオブジェクトに保存される日付のフォーマットの
有効な値は次のとおりです。:

* ``string`` (例 ``12:17:26``)
* ``datetime`` (単一の ``DateTime`` オブジェクト)
* ``array`` (例 ``array('hour' => 12, 'minute' => 17, 'second' => 26)``)
* ``timestamp`` (例 ``1307232000``)

フォームから戻って来る値も、この形式に戻して正規化されます。

.. include:: /reference/forms/types/options/with_minutes.rst.inc

.. include:: /reference/forms/types/options/with_seconds.rst.inc

.. include:: /reference/forms/types/options/hours.rst.inc

.. include:: /reference/forms/types/options/minutes.rst.inc

.. include:: /reference/forms/types/options/seconds.rst.inc

.. include:: /reference/forms/types/options/model_timezone.rst.inc

.. include:: /reference/forms/types/options/view_timezone.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

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
----------

以下のオプションは :doc:`form </reference/forms/types/form>` タイプを継承しています:

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/inherit_data.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

フォーム変数
------------

+--------------+-------------+---------------------------------------------------------------------------+
| 変数         | データ型    | 使用法                                                                    |
+==============+=============+===========================================================================+
| widget       | ``mixed``   | `widget`_ オプションの値。                                                |
+--------------+-------------+---------------------------------------------------------------------------+
| with_minutes | ``Boolean`` | `with_minutes`_ オプションの値。                                          |
+--------------+-------------+---------------------------------------------------------------------------+
| with_seconds | ``Boolean`` | `with_seconds`_ オプションの値。                                          |
+--------------+-------------+---------------------------------------------------------------------------+
| type         | ``string``  | ``widget`` が ``single_text`` で、且つ、 HTML5 が有効な場合のみ、         |
|              |             | インプットタイプ(``datetime``, ``date`` または ``time`` )を使用できます。 |
+--------------+-------------+---------------------------------------------------------------------------+


.. 2014/04/23 yositani2002 d49d12eaf265a5d6d32ac660c62f385d57261475
