.. index::
   single: Forms; Fields; url

.. note::

    * 対象バージョン：2.4 (2.0以降)
    * 翻訳更新日：2013/12/22

url フィールドタイプ
====================

``url`` フィールドはテキストフィールドです。送信された値にプロトコルが付いていない場合、指定されたプロトコル(例 ``http://`` )を送信された値の前に追加します。

+-------------+-------------------------------------------------------------------+
| 対応するタグ| ``input url`` フィールド                                          |
+-------------+-------------------------------------------------------------------+
| オプション  | - `default_protocol`_                                             |
+-------------+-------------------------------------------------------------------+
| 継承された  | - `max_length`_                                                   |
| オプション  | - `required`_                                                     |
|             | - `label`_                                                        |
|             | - `trim`_                                                         |
|             | - `read_only`_                                                    |
|             | - `disabled`_                                                     |
|             | - `error_bubbling`_                                               |
|             | - `error_mapping`_                                                |
|             | - `mapped`_                                                       |
+-------------+-------------------------------------------------------------------+
| 親タイプ    | :doc:`text </reference/forms/types/text>`                         |
+-------------+-------------------------------------------------------------------+
| クラス      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\UrlType` |
+-------------+-------------------------------------------------------------------+

フィールドオプション
--------------------

default_protocol
~~~~~~~~~~~~~~~~

**データ型**: ``string`` **デフォルト**: ``http``

送信された値が多数のプロトコル (例 ``http://``,``ftp://``, など)で始まらない場合、このプロトコルがフォーム送信された文字列の前に付加されます。

継承されたオプション
--------------------

以下のオプションは :doc:`form </reference/forms/types/form>` タイプを継承しています:

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. 2013/12/22 yositani2002 b675661199d466be4b6cb6f70d16aa1e3574c789
