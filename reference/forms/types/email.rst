.. index::
   single: Forms; Fields; email

.. note::

    * 対象バージョン：2.3 (2.1以降)
    * 翻訳更新日：2013/11/23

email フィールドタイプ
================

``email`` フォールドはHTML5の ``<input type="email" />`` タグで出力されるテキストフィールドです。

+-------------+---------------------------------------------------------------------+
| 対応するタグ | ``input`` ``email`` フィールド(テキストボックス)                              |
+-------------+---------------------------------------------------------------------+
| 継承された  | - `max_length`_                                                     |
| オプション  | - `required`_                                                       |
|             | - `label`_                                                          |
|             | - `trim`_                                                           |
|             | - `read_only`_                                                      |
|             | - `disabled`_                                                       |
|             | - `error_bubbling`_                                                 |
|             | - `error_mapping`_                                                  |
|             | - `mapped`_                                                         |
+-------------+---------------------------------------------------------------------+
| 親タイプ    | :doc:`form </reference/forms/types/form>`                           |
+-------------+---------------------------------------------------------------------+
| クラス      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\EmailType` |
+-------------+---------------------------------------------------------------------+

継承されたオプション
-----------------

これらのオプションは :doc:`form </reference/forms/types/form>` タイプを継承しています:

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. 2013/11/23 sotarof 4848f40b7de1463e40911bc2871d8990757d0097
