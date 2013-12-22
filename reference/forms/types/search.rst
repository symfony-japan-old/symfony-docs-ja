.. index::
   single: Forms; Fields; search

.. note::

    * 対象バージョン：2.4 (2.0以降)
    * 翻訳更新日：2013/12/22

search フィールドタイプ
=======================

このタイプは ``<input type="search" />`` フィールドを表示します。それはいくつかのブラウザでサポートされている特別な機能を持ったテキストボックスです。

`DiveIntoHTML5.info`_ の ``search`` フィールドを参照してください。

+-------------+----------------------------------------------------------------------+
| 対応するタグ| ``input search`` フィールド                                          |
+-------------+----------------------------------------------------------------------+
| 継承された  | - `max_length`_                                                      |
| オプション  | - `required`_                                                        |
|             | - `label`_                                                           |
|             | - `trim`_                                                            |
|             | - `read_only`_                                                       |
|             | - `disabled`_                                                        |
|             | - `error_bubbling`_                                                  |
|             | - `error_mapping`_                                                   |
|             | - `mapped`_                                                          |
+-------------+----------------------------------------------------------------------+
| 親タイプ    | :doc:`text </reference/forms/types/text>`                            |
+-------------+----------------------------------------------------------------------+
| クラス      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\SearchType` |
+-------------+----------------------------------------------------------------------+

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

.. _`DiveIntoHTML5.info`: http://diveintohtml5.info/forms.html#type-search

.. 2013/12/22 yositani2002 4848f40b7de1463e40911bc2871d8990757d0097
