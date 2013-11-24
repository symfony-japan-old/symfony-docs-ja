.. index::
   single: Forms; Fields; text

.. note::

   * 対象バージョン：2.3
   * 翻訳更新日：2013/11/23

text フィールドタイプ
=====================

textフィールドは最もベーシックなインプットテキストフィールドを表示します。

+-------------+--------------------------------------------------------------------+
| 対応するタグ| ``input`` ``text`` フィールド                                      |
+-------------+--------------------------------------------------------------------+
| 継承された  | - `max_length`_                                                    |
| オプション  | - `required`_                                                      |
|             | - `label`_                                                         |
|             | - `trim`_                                                          |
|             | - `read_only`_                                                     |
|             | - `disabled`_                                                      |
|             | - `error_bubbling`_                                                |
|             | - `error_mapping`_                                                 |
|             | - `mapped`_                                                        |
+-------------+--------------------------------------------------------------------+
| 親タイプ    | :doc:`field</reference/forms/types/form>`                          |
+-------------+--------------------------------------------------------------------+
| クラス      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\TextType` |
+-------------+--------------------------------------------------------------------+


継承されたオプション
--------------------

以下のオプションは :doc:`form </reference/forms/types/form>` タイプから継承します:

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. 2013/11/23 kseta 4848f40b7de1463e40911bc2871d8990757d0097
