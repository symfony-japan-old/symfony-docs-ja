.. note::

    * 対象バージョン：2.3 (2.1以降)
    * 翻訳更新日：2013/11/23

.. index::
   single: Forms; Fields; hidden

hidden フィールドタイプ
=======================

hidden タイプ は hidden の input フィールドを意味しています。

+--------------+----------------------------------------------------------------------+
| 対応するタグ | ``input`` ``hidden`` フィールド                                      |
+--------------+----------------------------------------------------------------------+
| 上書きされた | - `required`_                                                        |
| オプション   | - `error_bubbling`_                                                  |
+--------------+----------------------------------------------------------------------+
| 継承された   | - `data`_                                                            |
| オプション   | - `property_path`_                                                   |
|              | - `mapped`_                                                          |
|              | - `error_mapping`_                                                   |
+--------------+----------------------------------------------------------------------+
| 親タイプ     | :doc:`form </reference/forms/types/form>`                            |
+--------------+----------------------------------------------------------------------+
| クラス       | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\HiddenType` |
+--------------+----------------------------------------------------------------------+

上書きされたオプション
----------------------

required
~~~~~~~~

**デフォルト**: ``false``

Hiddenフィールドは必須にできません。

error_bubbling
~~~~~~~~~~~~~~

**デフォルト**: ``true``

ルートのフォームにエラーを渡してください。そうしないとエラーは見えないでしょう。

継承されたオプション
--------------------

以下のオプションは :doc:`form </reference/forms/types/form>` タイプを継承しています:

.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/property_path.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc


.. yositani2002 4848f40b7de1463e40911bc2871d8990757d0097
