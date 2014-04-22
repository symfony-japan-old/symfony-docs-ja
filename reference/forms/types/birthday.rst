.. index::
   single: Forms; Fields; birthday

.. note::

   * 対象バージョン：2.3+
   * 翻訳更新日：2014/04/23

birthday フィールドタイプ
=========================

:doc:`date </reference/forms/types/date>` フィールドは誕生日のデータを取り扱うことに特化しています。

1つのテキストボックス、または、3つ(年月日)のテキストボックス、または、3つのセレクトボックスとして表示されます。

このタイプは :doc:`date </reference/forms/types/date>` タイプと基本的に同じですが、より適切なデフォルトを  `years`_ オプションにもちます。
`years`_ オプションにはデフォルトで 120年前から今年まで（の選択肢が入ります）。

+------------------------+-------------------------------------------------------------------------------------------+
| 対応するデータ型       | ``DateTime``, ``string``, ``timestamp``, ``array``                                        |
|                        | (参照 :ref:`input option <form-reference-date-input>`)                                    |
+------------------------+-------------------------------------------------------------------------------------------+
| 対応するタグ           | `widget`_ オプションに基づいた3つのセレクトボックス、または、1つか3つのテキストボックス   |
+------------------------+-------------------------------------------------------------------------------------------+
| 上書きされたオプション | - `years`_                                                                                |
+------------------------+-------------------------------------------------------------------------------------------+
| 継承されたオプション   | - `widget`_                                                                               |
|                        | - `input`_                                                                                |
|                        | - `empty_value`_                                                                          |
|                        | - `months`_                                                                               |
|                        | - `days`_                                                                                 |
|                        | - `format`_                                                                               |
|                        | - `model_timezone`_                                                                       |
|                        | - `view_timezone`_                                                                        |
|                        | - `data`_                                                                                 |
|                        | - `invalid_message`_                                                                      |
|                        | - `invalid_message_parameters`_                                                           |
|                        | - `read_only`_                                                                            |
|                        | - `disabled`_                                                                             |
|                        | - `mapped`_                                                                               |
|                        | - `inherit_data`_                                                                         |
+------------------------+-------------------------------------------------------------------------------------------+
| 親タイプ               | :doc:`date </reference/forms/types/date>`                                                 |
+------------------------+-------------------------------------------------------------------------------------------+
| クラス                 | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\BirthdayType`                    |
+------------------------+-------------------------------------------------------------------------------------------+

上書きされたオプション
----------------------

years
~~~~~

**データ型**: ``array`` **デフォルト**: 120年前から今年まで

``year``  フィールドタイプで使用可能な年のリストです。このオプションは ``widget`` オプションが ``choice`` のときにのみ関連があります。

継承されたオプション
--------------------

以下のオプションは :doc:`date </reference/forms/types/date>` タイプを継承しています:

.. include:: /reference/forms/types/options/date_widget.rst.inc

.. include:: /reference/forms/types/options/date_input.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

.. include:: /reference/forms/types/options/months.rst.inc

.. include:: /reference/forms/types/options/days.rst.inc

.. include:: /reference/forms/types/options/date_format.rst.inc

.. include:: /reference/forms/types/options/model_timezone.rst.inc

.. include:: /reference/forms/types/options/view_timezone.rst.inc

以下のオプションは :doc:`form </reference/forms/types/form>` タイプを継承しています:

.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/inherit_data.rst.inc

.. 2014/04/23 yositani2002 d49d12eaf265a5d6d32ac660c62f385d57261475
